#include <gtest/gtest.h>

#include <counting_sort.hpp>
#include <order.hpp>

namespace proud_color_sorter::tests {

TEST(OrderTests, order) {
  Order<Color, kColorCount> colors_order;
  colors_order.Set(Color::kBlue, 0);
  colors_order.Set(Color::kGreen, 1);
  colors_order.Set(Color::kRed, 2);

  EXPECT_EQ(colors_order.GetElement(0), Color::kBlue);
  EXPECT_EQ(colors_order.GetElement(1), Color::kGreen);
  EXPECT_EQ(colors_order.GetElement(2), Color::kRed);

  EXPECT_EQ(colors_order.GetRank(Color::kBlue), 0);
  EXPECT_EQ(colors_order.GetRank(Color::kGreen), 1);
  EXPECT_EQ(colors_order.GetRank(Color::kRed), 2);
}

TEST(OrderTests, iterator) {
  Order<Color, kColorCount> colors_order;
  colors_order.Set(Color::kBlue, 0);
  colors_order.Set(Color::kGreen, 1);
  colors_order.Set(Color::kRed, 2);

  auto first = colors_order.begin();
  EXPECT_EQ(*first, Color::kBlue);

  auto second = first++;
  EXPECT_EQ(*second, Color::kBlue);

  auto third = second++;
  EXPECT_EQ(*third, Color::kBlue);

  EXPECT_EQ(*(--second), Color::kBlue);
}

TEST(OrderTests, const_iterator) {

}

}  // namespace proud_color_sorter::tests