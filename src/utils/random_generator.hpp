#pragma once

#include <cstdint>
#include <random>
#include <type_traits>

namespace proud_color_sorter::utils {

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
class RandomGenerator {
 public:
  RandomGenerator(T min_value, T max_value)
      : uniform_distribution_(min_value, max_value), mersenne_twister_(rand_device_()) {}

  T Generate() { return uniform_distribution_(mersenne_twister_); }

 private:
  std::random_device rand_device_;
  std::mt19937 mersenne_twister_;
  std::uniform_int_distribution<std::size_t> uniform_distribution_;
};

}  // namespace proud_color_sorter::utils