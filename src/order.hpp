#pragma once

#include <array>
#include <cstddef>
#include <iterator>
#include <unordered_map>

namespace proud_color_sorter {

/// Stores order of elements of type \a T.
/// Provides STL-like iterators API to iterate through order relation.
template <typename T, std::size_t MaxRank>
class Order {
 public:
  /// Mutable iterator.
  class Iterator;

  /// Immutable iterator.
  class ConstIterator;

  /// Sets map between \a element and \a rank.
  void Set(T element, const std::size_t rank) noexcept;

  /// Returns rank of \a element
  std::size_t GetRank(const T& element) const noexcept;

  /// Returns element by \a rank
  T& GetElement(const std::size_t rank) noexcept;

  /// Returns const reference to element by \a rank
  const T& GetElement(const std::size_t rank) const noexcept;

  /// Returns \c true if \a lhs rank is equal to \a rhs rank, returns \c false otherwise.
  bool IsEqual(const T& lhs, const T& rhs) const noexcept;

  /// Returns \c true if \a lhs is not equal to \a rhs rank, returns \c false otherwise.
  bool IsNotEqual(const T& lhs, const T& rhs) const noexcept;

  /// Returns \c true if \a lhs rank is less than \a rhs rank, returns \c false otherwise.
  bool IsLess(const T& lhs, const T& rhs) const noexcept;

  /// Returns \c true if \a lhs rank is less or equal to \a rhs rank, returns \c false otherwise.
  bool IsLessOrEqual(const T& lhs, const T& rhs) const noexcept;

  /// Returns \c true if \a lhs is greater than \a rhs, returns \c false otherwise.
  bool IsGreater(const T& lhs, const T& rhs) const noexcept;

  /// Returns \c true if \a lhs is greater or less than \a rhs, returns \c false otherwise.
  bool IsGreaterOrEqual(const T& lhs, const T& rhs) const noexcept;

  /// Returns \a Iterator pointing to the beginning of order.
  Iterator begin() { return Iterator{rank_to_element_.data()}; }  // NOLINT

  /// Returns \a Iterator pointing to the element after the last.
  Iterator end() { return Iterator{rank_to_element_.data() + MaxRank}; }  // NOLINT

  /// Returns \a ConstIterator pointing to the fist element.
  ConstIterator cbegin() const { return ConstIterator{rank_to_element_.data()}; }  // NOLINT

  /// Returns \a ConstIterator pointing to the element after the last.
  ConstIterator cend() const { return ConstIterator{rank_to_element_.data() + MaxRank}; }  // NOLINT

  /// Returns \a ConstIterator pointing to the fist element.
  ConstIterator begin() const { return ConstIterator{rank_to_element_.data()}; }  // NOLINT

  /// Returns \a ConstIterator pointing to the element after the last.
  ConstIterator end() const { return ConstIterator{rank_to_element_.data() + MaxRank}; }  // NOLINT

 private:
  std::array<T, MaxRank> rank_to_element_;
  std::unordered_map<T, std::size_t> element_to_rank_;
};

template <typename T, std::size_t MaxRank>
class Order<T, MaxRank>::Iterator {
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

  Iterator operator++(int) {
    Iterator it = Iterator{rank_to_element_ptr_};
    ++rank_to_element_ptr_;
    return it;
  }

  Iterator& operator--() {
    --rank_to_element_ptr_;
    return *this;
  }

  Iterator operator--(int) {
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

  reference operator[](difference_type index) {
    return *(rank_to_element_ptr_ + index);
  }

  friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
    return lhs.rank_to_element_ptr_ == rhs.rank_to_element_ptr_;
  }

  friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
    return lhs.rank_to_element_ptr_ != rhs.rank_to_element_ptr_;
  }

  friend bool operator<(const Iterator& lhs, const Iterator& rhs) {
    return lhs.rank_to_element_ptr_ < rhs.rank_to_element_ptr_;
  }

  friend bool operator<=(const Iterator& lhs, const Iterator& rhs) {
    return lhs.rank_to_element_ptr_ <= rhs.rank_to_element_ptr_;
  }

  friend bool operator>(const Iterator& lhs, const Iterator& rhs) {
    return lhs.rank_to_element_ptr_ > rhs.rank_to_element_ptr_;
  }

  friend bool operator>=(const Iterator& lhs, const Iterator& rhs) {
    return lhs.rank_to_element_ptr_ >= rhs.rank_to_element_ptr_;
  }

  friend Iterator operator+(const Iterator& lhs, difference_type rhs) {
    return Iterator{lhs.rank_to_element_ptr_ + rhs};
  }

  friend Iterator operator+(difference_type lhs, const Iterator& rhs) {
    return Iterator{rhs.rank_to_element_ptr_ + lhs};
  }

  friend Iterator operator-(const Iterator& lhs, difference_type rhs) {
    return Iterator{lhs.rank_to_element_ptr_ - rhs};
  }

  friend difference_type operator-(const Iterator& lhs, const Iterator& rhs) {
    return lhs.rank_to_element_ptr_ - rhs.rank_to_element_ptr_;
  }

 private:
  T* rank_to_element_ptr_ = nullptr;
};

template <typename T, std::size_t MaxRank>
class Order<T, MaxRank>::ConstIterator {
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

  ConstIterator operator++(int) {
    ConstIterator it{rank_to_element_ptr_};
    ++rank_to_element_ptr_;
    return it;
  }

  ConstIterator& operator--() {
    --rank_to_element_ptr_;
    return *this;
  }

  ConstIterator operator--(int) {
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

  reference operator[](difference_type index) const {
    return *(rank_to_element_ptr_ + index);
  }

  friend bool operator==(const ConstIterator& lhs, const ConstIterator& rhs) {
    return lhs.rank_to_element_ptr_ == rhs.rank_to_element_ptr_;
  }

  friend bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs) {
    return lhs.rank_to_element_ptr_ != rhs.rank_to_element_ptr_;
  }

  friend bool operator<(const ConstIterator& lhs, const ConstIterator& rhs) {
    return lhs.rank_to_element_ptr_ < rhs.rank_to_element_ptr_;
  }

  friend bool operator<=(const ConstIterator& lhs, const ConstIterator& rhs) {
    return lhs.rank_to_element_ptr_ <= rhs.rank_to_element_ptr_;
  }

  friend bool operator>(const ConstIterator& lhs, const ConstIterator& rhs) {
    return lhs.rank_to_element_ptr_ > rhs.rank_to_element_ptr_;
  }

  friend bool operator>=(const ConstIterator& lhs, const ConstIterator& rhs) {
    return lhs.rank_to_element_ptr_ >= rhs.rank_to_element_ptr_;
  }

  friend ConstIterator operator+(const ConstIterator& lhs, difference_type rhs) {
    return ConstIterator{lhs.rank_to_element_ptr_ + rhs};
  }

  friend ConstIterator operator+(difference_type lhs, const ConstIterator& rhs) {
    return ConstIterator{rhs.rank_to_element_ptr_ + lhs};
  }

  friend ConstIterator operator-(const ConstIterator& lhs, difference_type rhs) {
    return ConstIterator{lhs.rank_to_element_ptr_ - rhs};
  }

  friend difference_type operator-(const ConstIterator& lhs, const ConstIterator& rhs) {
    return lhs.rank_to_element_ptr_ - rhs.rank_to_element_ptr_;
  }

 private:
  const T* rank_to_element_ptr_ = nullptr;
};

template <typename T, std::size_t MaxRank>
void Order<T, MaxRank>::Set(T element, const std::size_t rank) noexcept {
  rank_to_element_[rank] = std::move(element);
  element_to_rank_[element] = rank;
}

template <typename T, std::size_t MaxRank>
std::size_t Order<T, MaxRank>::GetRank(const T& element) const noexcept {
  return element_to_rank_.at(element);
}

template <typename T, std::size_t MaxRank>
T& Order<T, MaxRank>::GetElement(const std::size_t rank) noexcept {
  return rank_to_element_[rank];
}

template <typename T, std::size_t MaxRank>
const T& Order<T, MaxRank>::GetElement(const std::size_t rank) const noexcept {
  return rank_to_element_[rank];
}

template <typename T, std::size_t MaxRank>
bool Order<T, MaxRank>::IsEqual(const T& lhs, const T& rhs) const noexcept {
  return element_to_rank_.at(lhs) == element_to_rank_.at(rhs);
}

template <typename T, std::size_t MaxRank>
bool Order<T, MaxRank>::IsNotEqual(const T& lhs, const T& rhs) const noexcept {
  return element_to_rank_[lhs] != element_to_rank_[rhs];
}

template <typename T, std::size_t MaxRank>
bool Order<T, MaxRank>::IsLess(const T& lhs, const T& rhs) const noexcept {
  return element_to_rank_.at(lhs) < element_to_rank_.at(rhs);
}

template <typename T, std::size_t MaxRank>
bool Order<T, MaxRank>::IsLessOrEqual(const T& lhs, const T& rhs) const noexcept {
  return element_to_rank_.at(lhs) <= element_to_rank_.at(rhs);
}

template <typename T, std::size_t MaxRank>
bool Order<T, MaxRank>::IsGreater(const T& lhs, const T& rhs) const noexcept {
  return element_to_rank_.at(lhs) > element_to_rank_.at(rhs);
}

template <typename T, std::size_t MaxRank>
bool Order<T, MaxRank>::IsGreaterOrEqual(const T& lhs, const T& rhs) const noexcept {
  return element_to_rank_.at(lhs) >= element_to_rank_.at(rhs);
}

}  // namespace proud_color_sorter
