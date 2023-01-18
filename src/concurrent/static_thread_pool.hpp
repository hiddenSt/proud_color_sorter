#pragma once

#include <functional>
#include <thread>
#include <vector>

#include <concurrent/mpmc_queue.hpp>

namespace proud_color_sorter::concurrent {

using Task = std::function<void()>;

class StaticThreadPool {
 public:
  explicit StaticThreadPool(std::size_t worker_count = std::thread::hardware_concurrency());
  ~StaticThreadPool();

  StaticThreadPool(const StaticThreadPool&) = delete;
  StaticThreadPool& operator=(const StaticThreadPool&) = delete;
  
  bool Submit(Task task);

  void Stop();
 
 private:
  void WorkerRoutine();

 private:
  std::vector<std::thread> workers_;
  MPMCUnboundedBlockingQueue<Task> task_queue_;
};

}  // namespace proud_color_sorter::concurrent
