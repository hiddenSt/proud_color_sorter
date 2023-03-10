#pragma once

#include <cstdlib>

#include <fmt/core.h>
#include <fmt/format.h>

#include <color.hpp>

template <>
struct fmt::formatter<proud_color_sorter::Color> {
  constexpr auto parse(format_parse_context& ctx) const -> decltype(ctx.begin()) {
    if (ctx.begin() != ctx.end()) {
      throw fmt::format_error{"Format specifications for 'Color' are not supported"};
    }

    return ctx.end();
  }

  template <typename FormatContext>
  auto format(const proud_color_sorter::Color color, FormatContext& ctx) const -> decltype(ctx.out()) {
    switch (color) {
      case proud_color_sorter::Color::kRed:
        return fmt::format_to(ctx.out(), "R");

      case proud_color_sorter::Color::kGreen:
        return fmt::format_to(ctx.out(), "G");

      case proud_color_sorter::Color::kBlue:
        return fmt::format_to(ctx.out(), "B");
    }

    // Unreachable code.
    // Probably, I must use the following macro:
    // https://github.com/hiddenSt/bicycle/blob/dev-initial-setup/bicycle/core/include/bicycle/contract.hpp#L41
    // cause it's readable.
    // But `bicycle` library is not ready yet.
    std::abort();
  }
};