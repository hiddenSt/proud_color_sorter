#pragma once

#include <array>
#include <cstddef>
#include <iterator>

namespace proud_color_sorter {
template <typename T, std::size_t MaxRank>
class Order {
 public:
  void Set(T element, const std::size_t rank) noexcept;
  std::size_t GetRank(const T& element) const noexcept;
  T& GetElement(const std::size_t rank) noexcept;
  const T& GetElement(const std::size_t rank) const noexcept;

  bool IsGreater(const T& lhs, const T& rhs) const noexcept;
  bool IsGreaterOrEqual(const T& lhs, const T& rhs) const noexcept;
  bool IsLess(const T& lhs, const T& rhs) const noexcept;
  bool IsLessOrEqual(const T& lhs, const T& rhs) const noexcept;
  bool IsEqual(const T& lhs, const T& rhs) const noexcept;
  bool IsNotEqual(const T& lhs, const T& rhs) const noexcept;

  class Iterator {
   public:
    using iterator_category = std::random_access_iterator_tag;  // NOLINT
    using value_type = T;                                       // NOLINT
    using pointer = T*;                                         // NOLINT
    using reference = T&;                                       // NOLINT
    using difference_type = std::ptrdiff_t;                     // NOLINT

    explicit Iterator(T* rank_to_element);

    reference operator*() const;
    pointer operator->();

    Iterator& operator++();
    Iterator operator++(int);

    friend bool operator==(const Iterator& lhs, const Iterator& rhs);
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs);

   private:
    T* rank_to_element_ptr_ = nullptr;
  };

  Iterator begin();
  Iterator end();

  Iterator rbegin();
  Iterator rend();

  // ConstIterator cbegin() const
  // ConstIterator cend() const
  // ConstIterator begin() const
  // ConstIterator end() const
  // Iterator rbegin()
  // Iterator rend()
  // ConstIterator crbegin() const
  // ConstIterator crend() const
  // ConstIterator rbegin() const
  // ConstIterator rend() const

 private:
  std::array<T, MaxRank> rank_to_element_;
  std::array<std::size_t, MaxRank> element_to_rank_;
};

template <typename T, std::size_t MaxRank>
void Order<T, MaxRank>::Set(T element, const std::size_t rank) noexcept {
  // Check if rank is less then MaxRank
  rank_to_element_[rank] = std::move(element);
  element_to_rank_[static_cast<std::size_t>(rank_to_element_[rank])] = rank;
}

template <typename T, std::size_t MaxRank>
std::size_t Order<T, MaxRank>::GetRank(const T& element) const noexcept {
  return element_to_rank_[static_cast<std::size_t>(rank_to_element_)];
}

template <typename T, std::size_t MaxRank>
T& Order<T, MaxRank>::GetElement(const std::size_t rank) noexcept {
  return rank_to_element_[rank];
}

template <typename T, std::size_t MaxRank>
const T& Order<T, MaxRank>::GetElement(const std::size_t rank) const noexcept {
  return rank_to_element_[rank];
}

}  // namespace proud_color_sorter