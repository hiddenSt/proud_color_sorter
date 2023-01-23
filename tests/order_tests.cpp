#include <gtest/gtest.h>

#include <color.hpp>
#include <counting_sort.hpp>
#include <order.hpp>

#include <utils.hpp>

namespace proud_color_sorter::tests {

TEST(OrderTests, order) {
  auto colors_order = utils::MakeOrder<Color, 3>({Color::kBlue, Color::kGreen, Color::kRed});

  EXPECT_EQ(colors_order.GetElement(0), Color::kBlue);
  EXPECT_EQ(colors_order.GetElement(1), Color::kGreen);
  EXPECT_EQ(colors_order.GetElement(2), Color::kRed);

  EXPECT_EQ(colors_order.GetRank(Color::kBlue), 0);
  EXPECT_EQ(colors_order.GetRank(Color::kGreen), 1);
  EXPECT_EQ(colors_order.GetRank(Color::kRed), 2);
}

TEST(OrderTests, random_iterator_requirements) {
  auto colors_order = utils::MakeOrder<Color, 3>({Color::kBlue, Color::kGreen, Color::kRed});

  auto first = colors_order.begin();
  EXPECT_EQ(*first, Color::kBlue);

  auto second = first++;
  EXPECT_EQ(*second, Color::kBlue);
  EXPECT_EQ(*first, Color::kGreen);

  auto third = second++;
  EXPECT_EQ(*third, Color::kBlue);

  third += 2;
  EXPECT_EQ(*third, Color::kRed);

  third -= 1;
  EXPECT_EQ(*third, Color::kGreen);

  third = ++first;
  EXPECT_EQ(*third, Color::kRed);
  EXPECT_EQ(*first, Color::kRed);
  EXPECT_EQ(first, third);
  EXPECT_NE(first, second);

  first = colors_order.begin();
  second = first + 1;
  third = first + 2;

  EXPECT_LT(first, second);
  EXPECT_LT(first, third);
  EXPECT_LT(second, third);
  EXPECT_EQ(second - first, 1);
  EXPECT_EQ(third - first, 2);
  EXPECT_EQ(first - first, 0);

  auto after_last = colors_order.end();
  EXPECT_NE(after_last, first);
  EXPECT_GT(after_last, first);
  EXPECT_GT(after_last, second);
  EXPECT_GT(after_last, first);

  EXPECT_EQ(first[0], *first);
  EXPECT_EQ(first[1], *second);
  EXPECT_EQ(first[2], *third);
}

TEST(OrderTests, const_random_iterator_requirements) {
  auto colors_order = utils::MakeOrder<Color, 3>({Color::kBlue, Color::kGreen, Color::kRed});

  auto first = colors_order.cbegin();
  EXPECT_EQ(*first, Color::kBlue);

  auto second = first++;
  EXPECT_EQ(*second, Color::kBlue);
  EXPECT_EQ(*first, Color::kGreen);

  auto third = second++;
  EXPECT_EQ(*third, Color::kBlue);

  third += 2;
  EXPECT_EQ(*third, Color::kRed);

  third -= 1;
  EXPECT_EQ(*third, Color::kGreen);

  third = ++first;
  EXPECT_EQ(*third, Color::kRed);
  EXPECT_EQ(*first, Color::kRed);
  EXPECT_EQ(first, third);
  EXPECT_NE(first, second);

  first = colors_order.cbegin();
  second = first + 1;
  third = first + 2;

  EXPECT_LT(first, second);
  EXPECT_LT(first, third);
  EXPECT_LT(second, third);
  EXPECT_EQ(second - first, 1);
  EXPECT_EQ(third - first, 2);
  EXPECT_EQ(first - first, 0);

  auto after_last = colors_order.cend();
  EXPECT_NE(after_last, first);
  EXPECT_GT(after_last, first);
  EXPECT_GT(after_last, second);
  EXPECT_GT(after_last, first);

  EXPECT_EQ(first[0], *first);
  EXPECT_EQ(first[1], *second);
  EXPECT_EQ(first[2], *third);
}

TEST(OrderTests, comparison_methods) {
  auto colors_order = utils::MakeOrder<Color, 3>({Color::kBlue, Color::kGreen, Color::kRed});

  EXPECT_TRUE(colors_order.IsEqual(Color::kBlue, Color::kBlue));
  EXPECT_FALSE(colors_order.IsEqual(Color::kBlue, Color::kRed));

  EXPECT_TRUE(colors_order.IsLess(Color::kBlue, Color::kRed));
  EXPECT_FALSE(colors_order.IsLess(Color::kRed, Color::kBlue));

  EXPECT_TRUE(colors_order.IsLessOrEqual(Color::kGreen, Color::kRed));
  EXPECT_TRUE(colors_order.IsLessOrEqual(Color::kGreen, Color::kGreen));
  EXPECT_FALSE(colors_order.IsLessOrEqual(Color::kGreen, Color::kBlue));

  EXPECT_TRUE(colors_order.IsGreater(Color::kRed, Color::kGreen));
  EXPECT_FALSE(colors_order.IsGreater(Color::kBlue, Color::kRed));

  EXPECT_TRUE(colors_order.IsGreaterOrEqual(Color::kGreen, Color::kBlue));
  EXPECT_TRUE(colors_order.IsGreaterOrEqual(Color::kGreen, Color::kGreen));
  EXPECT_FALSE(colors_order.IsGreaterOrEqual(Color::kBlue, Color::kRed));
}

TEST(OrderTest, iterator_preserves_elements_order) {
  Order<int, 5> order;
  std::vector<int> ordered_seq{3, 4, 6, 7, 9};

  for (std::size_t i = 0; i < ordered_seq.size(); ++i) {
    order.Set(ordered_seq[i], i);
  }

  std::vector<int> collected_from_iterator;

  for (int element : order) {
    collected_from_iterator.push_back(element);
  }

  EXPECT_EQ(ordered_seq, collected_from_iterator);
}

TEST(OrderTest, const_iterator_preserves_elements_order) {
  Order<int, 5> order;
  std::vector<int> ordered_seq{3, 4, 6, 7, 9};

  for (std::size_t i = 0; i < ordered_seq.size(); ++i) {
    order.Set(ordered_seq[i], i);
  }

  std::vector<int> collected_from_iterator;

  const Order<int, 5>& const_order = order;

  for (int element : const_order) {
    collected_from_iterator.push_back(element);
  }

  EXPECT_EQ(ordered_seq, collected_from_iterator);
}

}  // namespace proud_color_sorter::tests