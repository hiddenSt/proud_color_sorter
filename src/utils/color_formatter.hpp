#pragma once

#include <fmt/core.h>
#include <fmt/format.h>

#include <color.hpp>

template <>
struct fmt::formatter<proud_color_sorter::Color> {
  constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
    if (ctx.begin() != ctx.end()) {
      throw fmt::format_error{"Format specifications for 'Color' are not supported"};
    }

    return ctx.end();
  }

  template <typename FormatContext>
  auto format(const proud_color_sorter::Color& color, FormatContext& ctx) -> decltype(ctx.out()) {
    switch (color) {
      case proud_color_sorter::Color::kRed:
        return fmt::format_to(ctx.out(), "R");

      case proud_color_sorter::Color::kGreen:
        return fmt::format_to(ctx.out(), "G");

      case proud_color_sorter::Color::kBlue:
        return fmt::format_to(ctx.out(), "B");
    }
  }
};