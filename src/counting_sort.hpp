#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include <order.hpp>

namespace proud_color_sorter {

constexpr static std::size_t kColorCount = 3;

enum class Color : std::uint8_t {
  kRed,
  kGreen,
  kBlue,
};

using ColorOrder = Order<Color, kColorCount>;

// Note: This task can be solved using `std::sort` like this:
// ```cpp
// std::vector<Color> colors;
// ColorOrder color_order;
// std::sort(colors.begin(),
//           colors.end(),
//           [&color_order](const Color lhs, const Color rhs) {
//             return color_order.IsLess(lhs, rhs);
//                                            
// });
// ```
std::vector<Color> CountingSort(const std::vector<Color>& colors, 
                                const ColorOrder& color_order);

}  // namespace proud_color_sorter