#include <utils/daemon_main.hpp>

#include <array>
#include <cstdint>
#include <random>
#include <csignal>
#include <thread>

#include <fmt/core.h>
#include <fmt/format.h>
#include <CLI/CLI.hpp>

#include <color.hpp>
#include <counting_sort.hpp>
#include <order.hpp>
#include <spsc_queue.hpp>
#include <utils/color_formatter.hpp>
#include <utils/channel.hpp>

namespace proud_color_sorter::utils {

template <typename T>
struct RandomGenerator {
  RandomGenerator(T a, T b) : uniform_distribution(a, b), mersenne_twister(rand_device()) {}

  std::random_device rand_device;
  std::mt19937 mersenne_twister;
  std::uniform_int_distribution<std::size_t> uniform_distribution;

  T Generate() { return uniform_distribution(mersenne_twister); }
};

namespace detail {

std::vector<Color> GenerateColors(RandomGenerator<std::uint64_t>& size_generator,
                                  RandomGenerator<std::uint64_t>& color_generator) {
  auto color_size = size_generator.Generate();
  std::vector<Color> colors;
  colors.reserve(color_size);

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

struct Config {
  std::array<Color, kColorCount> color_order{Color::kGreen, Color::kBlue, Color::kRed};
  std::size_t generated_seq_max_size = 0;
};

void SignalHandler(int signal) {
  if (signal != SIGINT) {
    return;
  }

  ChannelSingleton::Get().GetChannel().Cancel();
}

}  // namespace detail

int DaemonMain(int argc, char* argv[]) {
  constexpr std::size_t kMaxGeneratedSequenceLength = 100;

  detail::Config config;
  CLI::App app{"App for sorting random generated colors."};
  app.add_option("--max_size", config.generated_seq_max_size, "Max length of generated color sequence.")
      ->default_val(kMaxGeneratedSequenceLength);
  // app.add_option("--colors_order", );
  CLI11_PARSE(app, argc, argv);

  ColorOrder color_order;

  for (std::size_t i = 0; i < config.color_order.size(); ++i) {
    color_order.Set(config.color_order[i], i);
  }

  Channel channel;
  ChannelSingleton::Get().Set(&channel);
  std::signal(SIGINT, ::proud_color_sorter::utils::detail::SignalHandler);

  auto producer = std::thread([&channel, config]() mutable {
    std::vector<Color> colors;
    RandomGenerator<std::uint64_t> size_generator{1, config.generated_seq_max_size};
    RandomGenerator<std::uint64_t> color_generator{0, kColorCount - 1};

    do {
      colors = detail::GenerateColors(size_generator, color_generator);
    } while (channel.Put(std::move(colors)));
  });

  detail::Consume(channel, color_order);
  fmt::print("Joining producer thread.");
  producer.join();

  return EXIT_SUCCESS;
}

}  // namespace proud_color_sorter::utils
