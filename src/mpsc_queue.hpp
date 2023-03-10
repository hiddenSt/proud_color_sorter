#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

namespace proud_color_sorter {

/// Multi-producer/Single-consumer (MPSC) unbounded blocking queue.
template <typename T>
class MPSCUnboundedBlockingQueue {
 public:
  /// Put an \a element to the queue if it's not closed and returns \c true, if the queue is closed does nothing and
  /// returns \c false.
  bool Put(T element);

  /// Returns element from the queue head if it's not closed and not empty.
  /// Blocks caller if the queue is empty and not closed until it's filled.
  /// In case if the queue is closed, returns \c std::optional containing \c std::nullopt
  std::optional<T> Take();

  /// Closes the queue for new \ref Put calls.
  void Close();

  /// Closes the queue. Drains pending elements from the queue.
  void Cancel();

 private:
  void CloseImpl(bool need_drain);

 private:
  std::queue<T> queue_;
  std::mutex queue_lock_;
  std::condition_variable queue_not_empty_;
  bool is_closed_{false};
};

template <typename T>
bool MPSCUnboundedBlockingQueue<T>::Put(T element) {
  {
    std::lock_guard lock{queue_lock_};

    if (is_closed_) {
      return false;
    }

    queue_.emplace(std::move(element));
  }

  queue_not_empty_.notify_one();

  return true;
}

template <typename T>
std::optional<T> MPSCUnboundedBlockingQueue<T>::Take() {
  std::unique_lock lock{queue_lock_};

  std::optional<T> element = std::nullopt;

  while (queue_.empty() && !is_closed_) {
    queue_not_empty_.wait(lock);
  }

  if (queue_.empty() && is_closed_) {
    return element;
  }

  element.emplace(std::move(queue_.front()));
  queue_.pop();

  return element;
}

template <typename T>
void MPSCUnboundedBlockingQueue<T>::Close() {
  CloseImpl(/*need_drain=*/false);
}

template <typename T>
void MPSCUnboundedBlockingQueue<T>::Cancel() {
  CloseImpl(/*need_drain=*/true);
}

template <typename T>
void MPSCUnboundedBlockingQueue<T>::CloseImpl(bool need_drain) {
  std::lock_guard lock{queue_lock_};
  is_closed_ = true;

  if (need_drain) {
    while (!queue_.empty()) {
      queue_.pop();
    }
  }

  queue_not_empty_.notify_one();
}

}  // namespace proud_color_sorter