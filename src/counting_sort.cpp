#include <counting_sort.hpp>

#include <cstdlib>

namespace proud_color_sorter {

namespace detail {

std::array<std::size_t, kColorCount> CountColors(const std::vector<Color>& colors) {
  std::array<std::size_t, kColorCount> color_count;

  for (std::size_t i = 0; i < color_count.size(); ++i) {
    color_count[i] = 0;
  }

  for (const auto& color : colors) {
    ++color_count[static_cast<std::size_t>(color)];
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
  auto color_count = detail::CountColors(colors);

  std::vector<Color> sorted_colors;
  sorted_colors.reserve(colors.size());

  for (const Color color : color_order) {
    detail::AddColorTo(sorted_colors, color_count[static_cast<std::size_t>(color)], color);
  }

  return sorted_colors;
}

}  // namespace proud_color_sorter