#include <gtest/gtest.h>

#include <cstdlib>
#include <limits>
#include <utils/daemon_main.hpp>

namespace proud_color_sorter::utils::tests {

TEST(DaemonMainTests, no_color_order_option) {
  const char* params[] = {"pcs"};
  EXPECT_NE(DaemonMain(1, params), EXIT_SUCCESS);
}

TEST(DaemonMainTests, invalid_color_order_element) {
  const char* params[] = {"pcs", "--color_order", "r", "h", "b"};
  EXPECT_NE(DaemonMain(5, params), EXIT_SUCCESS);
}

TEST(DaemonMainTests, color_order_option_duplicate_values) {
  const char* params[] = {"pcs", "--color_order", "r", "g", "g"};
  EXPECT_NE(DaemonMain(5, params), EXIT_SUCCESS);
}

TEST(DaemonMainTests, negative_value_for_max_size_option) {
  const char* params[] = {"pcs", "--max_size=-2"};
  EXPECT_NE(DaemonMain(2, params), EXIT_SUCCESS);
}

TEST(DaemonMainTests, not_a_number_for_max_size_option) {
  const char* params[] = {"pcs", "--max_size=aaaa"};
  EXPECT_NE(DaemonMain(2, params), EXIT_SUCCESS);
}

TEST(DaemonMainTests, invalid_number_of_colors_for_color_order_option) {
  const char* no_colors[] = {"pcs", "--color_order"};
  EXPECT_NE(DaemonMain(2, no_colors), EXIT_SUCCESS);

  const char* one_color[] = {"pcs", "--color_order", "r"};
  EXPECT_NE(DaemonMain(3, one_color), EXIT_SUCCESS);

  const char* two_colors[] = {"pcs", "--color_order", "r", "g"};
  EXPECT_NE(DaemonMain(4, two_colors), EXIT_SUCCESS);

  const char* more_than_three_colors[] = {"pcs", "--color_order", "r", "g", "g", "b"};
  EXPECT_NE(DaemonMain(6, more_than_three_colors), EXIT_SUCCESS);
}

}  // namespace proud_color_sorter::utils::tests