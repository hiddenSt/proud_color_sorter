#pragma once

#include <array>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <unordered_map>

namespace proud_color_sorter {

/// Stores order of elements of type \a T.
/// Provides STL-like iterators API to iterate through the order relation.
template <typename T, std::size_t SetSize>
class Order {
 public:
  /// A mutable iterator.
  class Iterator;

  /// An immutable iterator.
  class ConstIterator;

  Order() = default;

  Order(const Order& other) = default;

  Order(Order&& other) noexcept = default;

  Order& operator=(const Order& other) = default;

  Order& operator=(Order&& order) noexcept = default;

  ~Order() = default;

  /// Sets map between \a element and \a rank.
  void Set(T element, const std::size_t rank) noexcept;

  /// Returns rank of an \a element
  [[nodiscard]] std::size_t GetRank(const T& element) const noexcept;

  /// Returns an element, which rank is equal to \a rank
  T& GetElement(const std::size_t rank) noexcept;

  /// Returns a const reference to the element by \a rank
  const T& GetElement(const std::size_t rank) const noexcept;

  /// Returns \c true if \a lhs rank is equal to \a rhs rank, returns \c false otherwise.
  [[nodiscard]] bool IsEqual(const T& lhs, const T& rhs) const noexcept;

  /// Returns \c true if \a lhs is not equal to \a rhs rank, returns \c false otherwise.
  [[nodiscard]] bool IsNotEqual(const T& lhs, const T& rhs) const noexcept;

  /// Returns \c true if \a lhs rank is less than \a rhs rank, returns \c false otherwise.
  [[nodiscard]] bool IsLess(const T& lhs, const T& rhs) const noexcept;

  /// Returns \c true if \a lhs rank is less or equal to \a rhs rank, returns \c false otherwise.
  [[nodiscard]] bool IsLessOrEqual(const T& lhs, const T& rhs) const noexcept;

  /// Returns \c true if \a lhs is greater than \a rhs, returns \c false otherwise.
  [[nodiscard]] bool IsGreater(const T& lhs, const T& rhs) const noexcept;

  /// Returns \c true if \a lhs is greater or less than \a rhs, returns \c false otherwise.
  [[nodiscard]] bool IsGreaterOrEqual(const T& lhs, const T& rhs) const noexcept;

  /// Returns \a Iterator pointing to the beginning of order.
  [[nodiscard]] Iterator begin() { return Iterator{rank_to_element_.data()}; }  // NOLINT

  /// Returns \a Iterator pointing to the element after the last.
  [[nodiscard]] Iterator end() { return Iterator{rank_to_element_.data() + SetSize}; }  // NOLINT

  /// Returns \a ConstIterator pointing to the fist element.
  [[nodiscard]] ConstIterator cbegin() const { return ConstIterator{rank_to_element_.data()}; }  // NOLINT

  /// Returns \a ConstIterator pointing to the element after the last.
  [[nodiscard]] ConstIterator cend() const { return ConstIterator{rank_to_element_.data() + SetSize}; }  // NOLINT

  /// Returns \a ConstIterator pointing to the fist element.
  [[nodiscard]] ConstIterator begin() const { return ConstIterator{rank_to_element_.data()}; }  // NOLINT

  /// Returns \a ConstIterator pointing to the element after the last.
  [[nodiscard]] ConstIterator end() const { return ConstIterator{rank_to_element_.data() + SetSize}; }  // NOLINT

 private:
  std::array<T, SetSize> rank_to_element_;
  std::unordered_map<T, std::size_t> element_to_rank_;
};

template <typename T, std::size_t SetSize>
class Order<T, SetSize>::Iterator {
 public:
  using iterator_category = std::random_access_iterator_tag;  // NOLINT
  using value_type = T;                                       // NOLINT
  using pointer = value_type*;                                // NOLINT
  using reference = value_type&;                              // NOLINT
  using difference_type = std::ptrdiff_t;                     // NOLINT

  explicit Iterator(T* rank_to_element) : rank_to_element_ptr_(rank_to_element) {}

  reference operator*() const { return *rank_to_element_ptr_; }

  pointer operator->() { return rank_to_element_ptr_; }

  Iterator& operator++() {
    ++rank_to_element_ptr_;
    return *this;
  }

  [[nodiscard]] Iterator operator++(int) {
    Iterator it = Iterator{rank_to_element_ptr_};
    ++rank_to_element_ptr_;
    return it;
  }

  Iterator& operator--() {
    --rank_to_element_ptr_;
    return *this;
  }

  [[nodiscard]] Iterator operator--(int) {
    Iterator it = Iterator{rank_to_element_ptr_};
    --rank_to_element_ptr_;
    return it;
  }

  Iterator& operator+=(difference_type n) {
    rank_to_element_ptr_ += n;
    return *this;
  }

  Iterator& operator-=(difference_type n) {
    rank_to_element_ptr_ -= n;
    return *this;
  }

  reference operator[](difference_type index) { return *(rank_to_element_ptr_ + index); }

  [[nodiscard]] friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
    return lhs.rank_to_element_ptr_ == rhs.rank_to_element_ptr_;
  }

  [[nodiscard]] friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
    return lhs.rank_to_element_ptr_ != rhs.rank_to_element_ptr_;
  }

  [[nodiscard]] friend bool operator<(const Iterator& lhs, const Iterator& rhs) {
    return lhs.rank_to_element_ptr_ < rhs.rank_to_element_ptr_;
  }

  [[nodiscard]] friend bool operator<=(const Iterator& lhs, const Iterator& rhs) {
    return lhs.rank_to_element_ptr_ <= rhs.rank_to_element_ptr_;
  }

  [[nodiscard]] friend bool operator>(const Iterator& lhs, const Iterator& rhs) {
    return lhs.rank_to_element_ptr_ > rhs.rank_to_element_ptr_;
  }

  [[nodiscard]] friend bool operator>=(const Iterator& lhs, const Iterator& rhs) {
    return lhs.rank_to_element_ptr_ >= rhs.rank_to_element_ptr_;
  }

  [[nodiscard]] friend Iterator operator+(const Iterator& lhs, difference_type rhs) {
    return Iterator{lhs.rank_to_element_ptr_ + rhs};
  }

  [[nodiscard]] friend Iterator operator+(difference_type lhs, const Iterator& rhs) {
    return Iterator{rhs.rank_to_element_ptr_ + lhs};
  }

  [[nodiscard]] friend Iterator operator-(const Iterator& lhs, difference_type rhs) {
    return Iterator{lhs.rank_to_element_ptr_ - rhs};
  }

  [[nodiscard]] friend difference_type operator-(const Iterator& lhs, const Iterator& rhs) {
    return lhs.rank_to_element_ptr_ - rhs.rank_to_element_ptr_;
  }

 private:
  T* rank_to_element_ptr_ = nullptr;
};

template <typename T, std::size_t SetSize>
class Order<T, SetSize>::ConstIterator {
 public:
  using iterator_category = std::random_access_iterator_tag;  // NOLINT
  using value_type = const T;                                 // NOLINT
  using pointer = value_type*;                                // NOLINT
  using reference = value_type&;                              // NOLINT
  using difference_type = std::ptrdiff_t;                     // NOLINT

  explicit ConstIterator(const T* rank_to_element) : rank_to_element_ptr_(rank_to_element) {}

  reference operator*() const { return *rank_to_element_ptr_; }
  pointer operator->() const { return rank_to_element_ptr_; }

  ConstIterator& operator++() {
    ++rank_to_element_ptr_;
    return *this;
  }

  [[nodiscard]] ConstIterator operator++(int) {
    ConstIterator it{rank_to_element_ptr_};
    ++rank_to_element_ptr_;
    return it;
  }

  ConstIterator& operator--() {
    --rank_to_element_ptr_;
    return *this;
  }

  [[nodiscard]] ConstIterator operator--(int) {
    ConstIterator it{rank_to_element_ptr_};
    --rank_to_element_ptr_;
    return it;
  }

  ConstIterator& operator+=(difference_type n) {
    rank_to_element_ptr_ += n;
    return *this;
  }

  ConstIterator& operator-=(difference_type n) {
    rank_to_element_ptr_ -= n;
    return *this;
  }

  reference operator[](difference_type index) const { return *(rank_to_element_ptr_ + index); }

  [[nodiscard]] friend bool operator==(const ConstIterator& lhs, const ConstIterator& rhs) {
    return lhs.rank_to_element_ptr_ == rhs.rank_to_element_ptr_;
  }

  [[nodiscard]] friend bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs) {
    return lhs.rank_to_element_ptr_ != rhs.rank_to_element_ptr_;
  }

  [[nodiscard]] friend bool operator<(const ConstIterator& lhs, const ConstIterator& rhs) {
    return lhs.rank_to_element_ptr_ < rhs.rank_to_element_ptr_;
  }

  [[nodiscard]] friend bool operator<=(const ConstIterator& lhs, const ConstIterator& rhs) {
    return lhs.rank_to_element_ptr_ <= rhs.rank_to_element_ptr_;
  }

  [[nodiscard]] friend bool operator>(const ConstIterator& lhs, const ConstIterator& rhs) {
    return lhs.rank_to_element_ptr_ > rhs.rank_to_element_ptr_;
  }

  [[nodiscard]] friend bool operator>=(const ConstIterator& lhs, const ConstIterator& rhs) {
    return lhs.rank_to_element_ptr_ >= rhs.rank_to_element_ptr_;
  }

  [[nodiscard]] friend ConstIterator operator+(const ConstIterator& lhs, difference_type rhs) {
    return ConstIterator{lhs.rank_to_element_ptr_ + rhs};
  }

  [[nodiscard]] friend ConstIterator operator+(difference_type lhs, const ConstIterator& rhs) {
    return ConstIterator{rhs.rank_to_element_ptr_ + lhs};
  }

  [[nodiscard]] friend ConstIterator operator-(const ConstIterator& lhs, difference_type rhs) {
    return ConstIterator{lhs.rank_to_element_ptr_ - rhs};
  }

  [[nodiscard]] friend difference_type operator-(const ConstIterator& lhs, const ConstIterator& rhs) {
    return lhs.rank_to_element_ptr_ - rhs.rank_to_element_ptr_;
  }

 private:
  const T* rank_to_element_ptr_ = nullptr;
};

template <typename T, std::size_t SetSize>
void Order<T, SetSize>::Set(T element, const std::size_t rank) noexcept {
  rank_to_element_[rank] = std::move(element);
  element_to_rank_[element] = rank;
}

template <typename T, std::size_t SetSize>
std::size_t Order<T, SetSize>::GetRank(const T& element) const noexcept {
  return element_to_rank_.at(element);
}

template <typename T, std::size_t SetSize>
T& Order<T, SetSize>::GetElement(const std::size_t rank) noexcept {
  return rank_to_element_[rank];
}

template <typename T, std::size_t SetSize>
const T& Order<T, SetSize>::GetElement(const std::size_t rank) const noexcept {
  return rank_to_element_[rank];
}

template <typename T, std::size_t SetSize>
bool Order<T, SetSize>::IsEqual(const T& lhs, const T& rhs) const noexcept {
  return GetRank(lhs) == GetRank(rhs);
}

template <typename T, std::size_t SetSize>
bool Order<T, SetSize>::IsNotEqual(const T& lhs, const T& rhs) const noexcept {
  return GetRank(lhs) != GetRank(rhs);
}

template <typename T, std::size_t SetSize>
bool Order<T, SetSize>::IsLess(const T& lhs, const T& rhs) const noexcept {
  return GetRank(lhs) < GetRank(rhs);
}

template <typename T, std::size_t SetSize>
bool Order<T, SetSize>::IsLessOrEqual(const T& lhs, const T& rhs) const noexcept {
  return GetRank(lhs) <= GetRank(rhs);
}

template <typename T, std::size_t SetSize>
bool Order<T, SetSize>::IsGreater(const T& lhs, const T& rhs) const noexcept {
  return GetRank(lhs) > GetRank(rhs);
}

template <typename T, std::size_t SetSize>
bool Order<T, SetSize>::IsGreaterOrEqual(const T& lhs, const T& rhs) const noexcept {
  return GetRank(lhs) >= GetRank(rhs);
}

}  // namespace proud_color_sorter
