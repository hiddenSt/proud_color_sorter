#include <stdexcept>

#include <fmt/core.h>
#include <fmt/format.h>
#include <CLI/CLI.hpp>

#include <utils/app.hpp>

int main(int argc, char* argv[]) {
  constexpr std::size_t kMaxGeneratedSequenceLength = 100;

  proud_color_sorter::utils::Config config;
  CLI::App app{"App for sorting random generated colors."};
  app.add_option("--max_size", config.generated_seq_max_size, "Max length of generated color sequence.")
      ->default_val(kMaxGeneratedSequenceLength);
  // app.add_option("--colors_order", );
  CLI11_PARSE(app, argc, argv);

  try {
    proud_color_sorter::utils::RunApp(config);
  } catch (const std::exception& error) {
    fmt::print(stderr, "Exception caught: {}", error.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}