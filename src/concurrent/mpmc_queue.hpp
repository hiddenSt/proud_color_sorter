#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <optional>

namespace proud_color_sorter::concurrent {

template <typename T>
class MPMCUnboundedBlockingQueue {
 public:
  template <typename... Args>
  void Emplace(Args&&... args) {

  }

  bool Put(const T& element) {
    {
      std::scoped_lock lock(queue_lock_);

      if (is_closed_) {
        return false;
      }

      queue_.push(element);
    }
    
    not_empty_.notify_one();

    return true;
  }

  std::optional<T> Take() {
    std::unique_lock lock(queue_lock_);

    while (queue_.empty() && !is_closed_) {
      not_empty_.wait(lock);
    }

    if (is_closed_ && queue_.empty()) {
      return std::nullopt;
    }

    T element = std::move(queue_.front());
    queue_.pop();

    return element;
  }

  void Close() {
    CloseImpl(false);
  }

  void Cancel() {
    CloseImpl(true);
  }

 private:
  void CloseImpl(bool clear) {
    {
      std::scoped_lock lock(queue_lock_);
      is_closed_ = true;

      if (clear) {
        while (!queue_.empty()) {
            queue_.pop();
        }
      }

    }

    not_empty_.notify_all();
  }

 private:
  std::queue<T> queue_;
  std::mutex queue_lock_;
  std::condition_variable not_empty_;
  bool is_closed_{false};
};

}  // namespace proud_color_sorter