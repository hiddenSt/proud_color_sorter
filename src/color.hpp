#pragma once

#include <cstdint>

namespace proud_color_sorter {

constexpr static std::size_t kColorCount = 3;

enum class Color : std::uint8_t {
  kRed = 0,
  kGreen = 1,
  kBlue = 2,
};

}  // namespace proud_color_sorter