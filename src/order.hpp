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
    using pointer = value_type*;                                // NOLINT
    using reference = value_type&;                              // NOLINT
    using difference_type = std::ptrdiff_t;                     // NOLINT

    explicit Iterator(T* rank_to_element);

    reference operator*() const;
    pointer operator->();

    Iterator& operator++();
    Iterator operator++(int);

    Iterator& operator--();
    Iterator operator--(int);

    Iterator& operator+=(difference_type n);
    Iterator& operator-=(difference_type n);

    friend bool operator==(const Iterator& lhs, const Iterator& rhs);
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs);
    friend bool operator==(const Iterator& lhs, const Iterator& rhs);
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs);
    friend bool operator<(const Iterator& lhs, const Iterator& rhs);
    friend bool operator<=(const Iterator& lhs, const Iterator& rhs);
    friend bool operator>(const Iterator& lhs, const Iterator& rhs);
    friend bool operator>=(const Iterator& lhs, const Iterator& rhs);

    friend Iterator operator+(const Iterator& lhs, difference_type rhs);
    friend Iterator operator+(difference_type lhs, const Iterator& rhs);
    friend Iterator operator-(const Iterator& lhs, difference_type rhs);
    friend difference_type operator-(const Iterator& lhs, const Iterator& rhs);

   private:
    T* rank_to_element_ptr_ = nullptr;
  };

  class ConstIterator {
   public:
    using iterator_category = std::random_access_iterator_tag;  // NOLINT
    using value_type = const T;                                 // NOLINT
    using pointer = value_type*;                                // NOLINT
    using reference = value_type&;                              // NOLINT
    using difference_type = std::ptrdiff_t;                     // NOLINT

    explicit ConstIterator(const T* rank_to_element);

    reference operator*() const;
    pointer operator->() const;

    ConstIterator& operator++();
    ConstIterator operator++(int);

    ConstIterator& operator--();
    ConstIterator operator--(int);

    ConstIterator& operator+=(difference_type n);
    ConstIterator& operator-=(difference_type n);

    friend bool operator==(const ConstIterator& lhs, const ConstIterator& rhs);
    friend bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs);
    friend bool operator==(const ConstIterator& lhs, const ConstIterator& rhs);
    friend bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs);
    friend bool operator<(const ConstIterator& lhs, const ConstIterator& rhs);
    friend bool operator<=(const ConstIterator& lhs, const ConstIterator& rhs);
    friend bool operator>(const ConstIterator& lhs, const ConstIterator& rhs);
    friend bool operator>=(const ConstIterator& lhs, const ConstIterator& rhs);

    friend ConstIterator operator+(const ConstIterator& lhs, difference_type rhs);
    friend ConstIterator operator+(difference_type lhs, const ConstIterator& rhs);
    friend ConstIterator operator-(const ConstIterator& lhs, difference_type rhs);
    friend difference_type operator-(const ConstIterator& lhs, const ConstIterator& rhs);

   private:
    const T* rank_to_element_ptr_ = nullptr;
  };

  Iterator begin() { return Iterator{rank_to_element_.data()}; }  // NOLINT
  Iterator end() { return Iterator{}; };                          // NOLINT

  // Iterator rbegin();  // NOLINT
  // Iterator rend();    // NOLINT

  ConstIterator cbegin() const { return ConstIterator{rank_to_element_.data()}; };  // NOLINT
  ConstIterator cend() const { return ConstIterator{}; };                           // NOLINT
  ConstIterator begin() const { return ConstIterator{rank_to_element_.data()}; };   // NOLINT
  ConstIterator end() const { return ConstIterator{}; };                            // NOLINT
  // ConstIterator crbegin() const;  // NOLINT
  // ConstIterator crend() const;    // NOLINT
  // ConstIterator rbegin() const;   // NOLINT
  // ConstIterator rend() const;     // NOLINT

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
