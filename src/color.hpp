#pragma once

#include <cstdint>

namespace proud_color_sorter {

/// Should be up to date with \ref Color, else it will be UB to use the whole app.
constexpr static std::size_t kMaxColorValue = 3;

/// Possible colors.
enum class Color : std::uint8_t {
  kRed = 0,
  kGreen = 1,
  kBlue = 2,
};

}  // namespace proud_color_sorter