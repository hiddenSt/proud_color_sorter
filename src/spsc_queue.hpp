#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

namespace proud_color_sorter {

/// Single-producer/Single-consumer (SPSC) unbounded blocking queue.
template <typename T>
class SPSCUnboundedBlockingQueue {
 public:
  /// Put \a element in queue if it's not closed and returns true, if queue is closed returns false.
  bool Put(T element);

  /// Returns element from queue head if it's not closed.
  /// Blocks caller if queue is empty until it's filled.
  std::optional<T> Take();

  /// Close queue for new \ref Put.
  void Close();

  /// Close queue. Drains pending elements from queue.
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
bool SPSCUnboundedBlockingQueue<T>::Put(T element) {
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
std::optional<T> SPSCUnboundedBlockingQueue<T>::Take() {
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
void SPSCUnboundedBlockingQueue<T>::Close() {
  CloseImpl(/*need_drain=*/false);
}

template <typename T>
void SPSCUnboundedBlockingQueue<T>::Cancel() {
  CloseImpl(/*need_drain=*/true);
}

template <typename T>
void SPSCUnboundedBlockingQueue<T>::CloseImpl(bool need_drain) {
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