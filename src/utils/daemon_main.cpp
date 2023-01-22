#include <utils/daemon_main.hpp>

#include <cstdlib>
#include <set>
#include <stdexcept>
#include <vector>

#include <fmt/core.h>
#include <fmt/format.h>
#include <CLI/CLI.hpp>

#include <color.hpp>
#include <utils/app.hpp>

namespace proud_color_sorter {

namespace detail {
std::array<Color, kMaxColorValue> ParseColorOrderArg(const std::vector<char>& arg) {
  std::array<Color, kMaxColorValue> color_order;
  for (std::size_t i = 0; i < arg.size(); ++i) {
    if (arg[i] == 'r') {
      color_order[i] = proud_color_sorter::Color::kRed;
    } else if (arg[i] == 'g') {
      color_order[i] = proud_color_sorter::Color::kGreen;
    } else if (arg[i] == 'b') {
      color_order[i] = proud_color_sorter::Color::kBlue;
    } else {
      throw std::logic_error{"Invalid value for option '--colors_order'. Possible values: 'r, 'g', 'b''\n"};
    }
  }

  std::set<char> unique_color{arg.begin(), arg.end()};

  if (unique_color.size() != arg.size()) {
    throw std::logic_error{"Values for option '--colors_order' must be unique.\n"};
  }

  return color_order;
}

}  // namespace detail

int DaemonMain(int argc, char* argv[]) {
  constexpr std::size_t kMaxGeneratedSequenceLength = 100;

  proud_color_sorter::utils::Config config;
  std::vector<char> color_order;

  CLI::App app{"App for sorting random generated colors."};
  app.add_option("--max_size", config.generated_seq_max_size, "Max length of generated color sequence.")
      ->default_val(kMaxGeneratedSequenceLength);
  app.add_option("--colors_order", color_order, "Elements order. Possible values: 'r', 'g', 'b'")
      ->expected(config.color_order.size())
      ->required();
  CLI11_PARSE(app, argc, argv);

  try {
    config.color_order = proud_color_sorter::detail::ParseColorOrderArg(color_order);
  } catch (const std::exception& error) {
    fmt::print(stderr, error.what());
    return EXIT_FAILURE;
  }

  try {
    proud_color_sorter::utils::RunApp(config);
  } catch (const std::exception& error) {
    fmt::print(stderr, "Exception caught: {}", error.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
}  // namespace proud_color_sorter