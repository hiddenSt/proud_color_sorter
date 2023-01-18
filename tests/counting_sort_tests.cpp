#include <array>
#include <vector>

#include <gtest/gtest.h>

#include <counting_sort.hpp>

namespace proud_color_sorter::tests {

TEST(CountingSortTest, empty_array) {
  std::vector<Color> colors;
  std::array<Color, kColorCount> color_order = {Color::kBlue, Color::kRed, Color::kGreen};

  auto sorted_colors = CountingSort(colors, color_order);

  ASSERT_TRUE(sorted_colors.empty());
}

TEST(CountingSortTest, empty_order) {}

TEST(CountingSortTest, order_test) {}

}  // namespace proud_color_sorter::tests