#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

#include <counting_sort.hpp>
#include <order.hpp>

#include <utils.hpp>

namespace proud_color_sorter::tests {

TEST(CountingSortTest, empty_array) {
  std::vector<Color> colors;
  auto order = utils::MakeOrder<Color, 3>({Color::kRed, Color::kGreen, Color::kBlue});

  auto sorted_colors = CountingSort(colors, order);

  ASSERT_TRUE(sorted_colors.empty());
}

TEST(CountingSortTest, order) {
  std::vector<Color> colors{Color::kRed, Color::kGreen, Color::kBlue, Color::kGreen};
  auto order = utils::MakeOrder<Color, 3>({Color::kRed, Color::kGreen, Color::kBlue});

  auto sorted_colors = CountingSort(colors, order);
  std::sort(colors.begin(), colors.end(),
            [&order](const Color lhs, const Color rhs) mutable { return order.IsLess(lhs, rhs); });

  EXPECT_EQ(sorted_colors, colors);
}

TEST(CountingSortTest, descending_input) {
  std::vector<Color> colors{Color::kRed, Color::kRed, Color::kRed, Color::kBlue, Color::kBlue, Color::kGreen};
  auto order = utils::MakeOrder<Color, 3>({Color::kGreen, Color::kBlue, Color::kRed});

  auto sorted_colors = CountingSort(colors, order);

  std::sort(colors.begin(), colors.end(),
            [&order](const Color lhs, const Color rhs) mutable { return order.IsLess(lhs, rhs); });

  EXPECT_EQ(sorted_colors, colors);
}

TEST(CountingSortTest, ascending_input) {
  std::vector<Color> colors{Color::kRed, Color::kRed, Color::kRed, Color::kBlue, Color::kBlue, Color::kGreen};
  auto order = utils::MakeOrder<Color, 3>({Color::kRed, Color::kGreen, Color::kBlue});

  auto sorted_colors = CountingSort(colors, order);

  std::sort(colors.begin(), colors.end(),
            [&order](const Color lhs, const Color rhs) mutable { return order.IsLess(lhs, rhs); });

  EXPECT_EQ(sorted_colors, colors);
}

}  // namespace proud_color_sorter::tests
