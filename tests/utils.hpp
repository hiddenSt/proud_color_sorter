#pragma once

#include <array>

#include <order.hpp>

namespace proud_color_sorter::tests::utils {

template <typename T, std::size_t Size>
decltype(auto) MakeOrder(const std::array<T, Size>& elements) {
  Order<T, Size> order;

  for (std::size_t i = 0; i < elements.size(); ++i) {
    order.Set(elements[i], i);
  }

  return order;
}

}  // namespace proud_color_sorter::tests::utils