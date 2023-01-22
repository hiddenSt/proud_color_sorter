#pragma once

#include <array>
#include <cstdint>

#include <color.hpp>

namespace proud_color_sorter::utils {

struct Config {
  std::array<Color, kMaxColorValue> color_order{Color::kRed, Color::kGreen, Color::kBlue};
  std::size_t generated_seq_max_size = 0;
};

void RunApp(const Config& config);

}  // namespace proud_color_sorter::utils