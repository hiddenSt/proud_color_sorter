#include <color.hpp>
#include <color_formatter.hpp>

#include <fmt/format.h>
#include <gtest/gtest.h>

namespace proud_color_sorter::tests {

TEST(ColorFormatterTests, colors) {
  EXPECT_EQ(fmt::format("{}", Color::kRed), "R");
  EXPECT_EQ(fmt::format("{}", Color::kGreen), "G");
  EXPECT_EQ(fmt::format("{}", Color::kBlue), "B");
}

TEST(ColorFormatterTests, throws_on_specifications) {
  EXPECT_THROW(fmt::format("{:a}", Color::kRed), fmt::format_error);  // NOLINT
}

}  // namespace proud_color_sorter::tests