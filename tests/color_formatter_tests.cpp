#include <fmt/format.h>
#include <gtest/gtest.h>

#include <color.hpp>
#include <utils/color_formatter.hpp>

namespace proud_color_sorter::tests {

TEST(ColorFormatterTests, colors) {
  EXPECT_EQ(fmt::format("{}", Color::kRed), "R");
  EXPECT_EQ(fmt::format("{}", Color::kGreen), "G");
  EXPECT_EQ(fmt::format("{}", Color::kBlue), "B");
}

TEST(ColorFormatterTests, throws_on_specifications) {
  try {
    auto str = fmt::format("{:a}", Color::kRed);
    FAIL() << str;
  } catch (const fmt::format_error&) {
    SUCCEED();
  }
}

}  // namespace proud_color_sorter::tests