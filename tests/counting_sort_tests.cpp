#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

#include <counting_sort.hpp>
#include <order.hpp>

namespace proud_color_sorter::tests {

TEST(CountingSortTest, empty_array) {
  std::vector<Color> colors;
  ColorOrder order;
  order.Set(Color::kRed, 0);
  order.Set(Color::kGreen, 1);
  order.Set(Color::kBlue, 2);
  auto sorted_colors = CountingSort(colors, order);

  ASSERT_TRUE(sorted_colors.empty());
}

TEST(CountingSortTest, order) {
  std::vector<Color> colors{Color::kRed, Color::kGreen, Color::kBlue, Color::kGreen, Color::kBlue};
  ColorOrder order;
  order.Set(Color::kRed, 0);
  order.Set(Color::kGreen, 1);
  order.Set(Color::kBlue, 2);

  auto sorted_colors = CountingSort(colors, order);
  std::sort(colors.begin(), colors.end(),
            [&order](const Color lhs, const Color rhs) mutable { return order.IsLess(lhs, rhs); });

  EXPECT_EQ(sorted_colors, colors);
}

}  // namespace proud_color_sorter::tests
