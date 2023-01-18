#include <atomic>
#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include <concurrent/mpmc_queue.hpp>

namespace proud_color_sorter::concurrent::tests {

TEST(MPMCQueueTests, put) {
  MPMCUnboundedBlockingQueue<int> queue;
  ASSERT_TRUE(queue.Put(1));
}

TEST(MPMCQueueTests, take) {
  MPMCUnboundedBlockingQueue<int> queue;
  ASSERT_TRUE(queue.Put(1));

  auto element = queue.Take();
  ASSERT_TRUE(element.has_value());
  ASSERT_EQ(element.value(), 1);
}

TEST(MPMCQueueTests, fifo) {
  MPMCUnboundedBlockingQueue<int> queue;
  EXPECT_TRUE(queue.Put(1));
  EXPECT_TRUE(queue.Put(2));
  EXPECT_TRUE(queue.Put(3));

  auto element_1 = queue.Take();
  ASSERT_TRUE(element_1.has_value());
  EXPECT_EQ(element_1.value(), 1);

  auto element_2 = queue.Take();
  ASSERT_TRUE(element_2.has_value());
  EXPECT_EQ(element_2.value(), 2);

  auto element_3 = queue.Take();
  ASSERT_TRUE(element_3.has_value());
  EXPECT_EQ(element_3.value(), 3);
}

TEST(MPMCQueueTests, close) {
  MPMCUnboundedBlockingQueue<int> queue;
  EXPECT_TRUE(queue.Put(1));

  queue.Close();
  EXPECT_FALSE(queue.Put(2));

  auto element_1 = queue.Take();
  auto element_2 = queue.Take();
  EXPECT_TRUE(element_1.has_value());
  EXPECT_FALSE(element_2.has_value());
  EXPECT_EQ(element_1.value(), 1);
}

TEST(MPMCQueueTests, cancel) {
  MPMCUnboundedBlockingQueue<int> queue;
  EXPECT_TRUE(queue.Put(1));

  queue.Cancel();
  EXPECT_FALSE(queue.Put(2));

  auto element = queue.Take();
  EXPECT_FALSE(element.has_value());
}

TEST(MPMCQueueTests, take_blocked_on_empty_queue) {
  MPMCUnboundedBlockingQueue<int> queue;

  std::atomic<int> taken_element = 0;

  auto consumer = std::thread([&]() mutable {
    auto element = queue.Take();

    ASSERT_TRUE(element.has_value());
    taken_element.store(element.value());
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  ASSERT_EQ(taken_element.load(), 0);
  ASSERT_TRUE(queue.Put(1));

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  ASSERT_EQ(taken_element.load(), 1);

  consumer.join();
}

TEST(MPMCQueueTests, awakes_on_close) {
  MPMCUnboundedBlockingQueue<int> queue;

  std::atomic<bool> consumer_1_awaken = false;
  std::atomic<bool> consumer_2_awaken = false;

  auto consumer_1 = std::thread([&]() mutable {
    queue.Take();
    consumer_1_awaken.store(true);
  });

  auto consumer_2 = std::thread([&]() mutable {
    queue.Take();
    consumer_2_awaken.store(true);
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  EXPECT_FALSE(consumer_1_awaken.load());
  EXPECT_FALSE(consumer_1_awaken.load());

  queue.Close();
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  EXPECT_TRUE(consumer_1_awaken.load());
  EXPECT_TRUE(consumer_2_awaken.load());

  consumer_1.join();
  consumer_2.join();
}

TEST(MPMCUnboundedBlockingQueue, awakes_on_cancel) {
  MPMCUnboundedBlockingQueue<int> queue;

  std::atomic<bool> consumer_1_awaken = false;
  std::atomic<bool> consumer_2_awaken = false;

  auto consumer_1 = std::thread([&]() mutable {
    queue.Take();
    consumer_1_awaken.store(true);
  });

  auto consumer_2 = std::thread([&]() mutable {
    queue.Take();
    consumer_2_awaken.store(true);
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  EXPECT_FALSE(consumer_1_awaken.load());
  EXPECT_FALSE(consumer_1_awaken.load());

  queue.Cancel();
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  EXPECT_TRUE(consumer_1_awaken.load());
  EXPECT_TRUE(consumer_2_awaken.load());

  consumer_1.join();
  consumer_2.join();
}

TEST(MPMCUnboundedBlockingQueue, concurrent_calls) { MPMCUnboundedBlockingQueue<int> queue; }

}  // namespace proud_color_sorter::concurrent::tests