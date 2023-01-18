#include <concurrent/static_thread_pool.hpp>

#include <stdexcept>

namespace proud_color_sorter::concurrent {

StaticThreadPool::StaticThreadPool(std::size_t num_threads) {
  for (std::size_t i = 0; i < num_threads; ++i) {
    workers_.emplace_back([this]() mutable {
      WorkerRoutine();
    });  
  }
}

StaticThreadPool::~StaticThreadPool() {
  // TODO: check if thread pool is stopped.
}

bool StaticThreadPool::Submit(Task task) {
  return task_queue_.Put(std::move(task));
}

void StaticThreadPool::Stop() {
  task_queue_.Cancel();

  for (auto& worker: workers_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

void StaticThreadPool::WorkerRoutine() {
  while (true) {
    auto task = task_queue_.Take();

    if (!task.has_value()) {
        return;
    }

    try {
      task->operator()();
    } catch (const std::exception& error) {
      // TODO: report error.
    }
  }
}

}  // namespace proud_color_sorter::concurrent
