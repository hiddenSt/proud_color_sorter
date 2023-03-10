#pragma once

#include <vector>

#include <color.hpp>
#include <order.hpp>

namespace proud_color_sorter {

using ColorOrder = Order<Color, kColorSize>;

/// Sorts \a colors using \a color_order.
///
/// Creates a new vector of sorted colors.
/// \note: This task can be solved using `std::sort` like this:
/// ```cpp
/// std::vector<Color> colors;
/// ColorOrder color_order;
/// std::sort(colors.begin(),
///           colors.end(),
///           [&color_order](const Color lhs, const Color rhs) {
///             return color_order.IsLess(lhs, rhs);
///
/// });
/// ```
std::vector<Color> CountingSort(const std::vector<Color>& colors, const ColorOrder& color_order);

}  // namespace proud_color_sorter