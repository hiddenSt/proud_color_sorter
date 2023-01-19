#include <utils/color_formatter.hpp>
#include <utils/daemon_main.hpp>

#include <random>
#include <thread>

#include <fmt/core.h>
#include <fmt/format.h>
#include <CLI/CLI.hpp>

#include <color.hpp>
#include <counting_sort.hpp>
#include <order.hpp>
#include <spsc_queue.hpp>

namespace proud_color_sorter::utils {

using Channel = SPSCUnboundedBlockingQueue<std::vector<Color>>;

template <typename T>
struct RandomGenerator {
  RandomGenerator(T a, T b) : uniform_distribution(a, b), mersenne_twister(rand_device()) {}

  std::random_device rand_device;
  std::mt19937 mersenne_twister;
  std::uniform_int_distribution<std::size_t> uniform_distribution;

  T Generate() { return uniform_distribution(mersenne_twister); }
};

namespace detail {

std::vector<Color> GenerateColors(RandomGenerator<std::size_t>& size_generator,
                                  RandomGenerator<std::uint8_t>& color_generator) {
  auto color_size = size_generator.Generate();
  std::vector<Color> colors;
  colors.reserve(color_size);

  std::uniform_int_distribution<std::uint8_t> uniform_distribution_element{0, kColorCount};

  for (std::size_t i = 0; i < color_size; ++i) {
    auto generated = color_generator.Generate();
    colors.emplace_back(static_cast<Color>(generated));
  }

  return colors;
}

void Consume(Channel& channel, const ColorOrder& order) {
  while (true) {
    auto colors = channel.Take();

    if (!colors.has_value()) {
      return;
    }

    fmt::print("Generated colors (size={}): {} \n", colors->size(), fmt::join(colors.value(), " "));
    auto sorted_colors = CountingSort(colors.value(), order);
    fmt::print("Sorted colors (size={}): {} \n", sorted_colors.size(), fmt::join(sorted_colors, " "));
  }
}

}  // namespace detail

int DaemonMain(int argc, char* argv[]) {
  CLI::App app{"App for sorting random generated colors."};
  std::size_t generated_seq_max_size = 0;
  app.add_option("--max_size", generated_seq_max_size, "Max length of generated color sequence.")->default_val(100);
  // app.add_option("--colors_order", );
  CLI11_PARSE(app, argc, argv);

  Channel channel;
  ColorOrder color_order;
  color_order.Set(Color::kRed, 0);
  color_order.Set(Color::kGreen, 1);
  color_order.Set(Color::kBlue, 2);

  auto producer = std::thread([&channel, generated_seq_max_size]() mutable {
    std::vector<Color> colors;
    RandomGenerator<std::size_t> size_generator{1, generated_seq_max_size};
    RandomGenerator<std::uint8_t> color_generator{0, kColorCount - 1};

    do {
      colors = detail::GenerateColors(size_generator, color_generator);
    } while (channel.Put(std::move(colors)));
  });

  detail::Consume(channel, color_order);

  return EXIT_SUCCESS;
}

}  // namespace proud_color_sorter::utils
