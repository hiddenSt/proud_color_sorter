#include <counting_sort.hpp>

#include <array>
#include <cstdint>

namespace proud_color_sorter {

namespace detail {

std::array<std::size_t, kColorSize> CountColors(const std::vector<Color>& colors, const ColorOrder& order) {
  std::array<std::size_t, kColorSize> color_count;

  for (std::size_t i = 0; i < color_count.size(); ++i) {
    color_count[i] = 0;
  }

  for (const Color color : colors) {
    ++color_count[order.GetRank(color)];
  }

  return color_count;
}

void AddColorTo(std::vector<Color>& colors, std::size_t color_count, const Color color) {
  while (color_count > 0) {
    colors.emplace_back(color);
    --color_count;
  }
}

}  // namespace detail

std::vector<Color> CountingSort(const std::vector<Color>& colors, const ColorOrder& color_order) {
  auto color_count = detail::CountColors(colors, color_order);

  std::vector<Color> sorted_colors;
  sorted_colors.reserve(colors.size());

  for (const Color color : color_order) {
    detail::AddColorTo(sorted_colors, color_count[color_order.GetRank(color)], color);
  }

  return sorted_colors;
}

}  // namespace proud_color_sorter