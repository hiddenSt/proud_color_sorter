#include <atomic>
#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include <spsc_queue.hpp>

namespace proud_color_sorter::tests {

TEST(SPSCQueueTests, put) {
  SPSCUnboundedBlockingQueue<int> queue;

  ASSERT_TRUE(queue.Put(1));
}

TEST(SPSCQueueTests, take_on_not_empty_queue) {
  SPSCUnboundedBlockingQueue<int> queue;
  ASSERT_TRUE(queue.Put(1));

  auto element = queue.Take();
  ASSERT_TRUE(element.has_value());
  ASSERT_EQ(element.value(), 1);
}

TEST(SPSCQueueTests, take_blocks_caller_on_empty_queue) {
  SPSCUnboundedBlockingQueue<int> queue;
  std::atomic<bool> element_taken = false;

  auto consumer = std::thread([&]() mutable {
    auto element = queue.Take();
    EXPECT_TRUE(element.has_value());
    element_taken.store(true);
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  EXPECT_FALSE(element_taken.load());
  EXPECT_TRUE(queue.Put(1));

  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  EXPECT_TRUE(element_taken.load());

  consumer.join();
}

TEST(SPSCQueueTests, close) {
  SPSCUnboundedBlockingQueue<int> queue;

  EXPECT_TRUE(queue.Put(1));
  queue.Close();
  EXPECT_FALSE(queue.Put(2));
  auto element_1 = queue.Take();
  ASSERT_TRUE(element_1.has_value());
  EXPECT_EQ(element_1.value(), 1);
  auto element_2 = queue.Take();
  EXPECT_FALSE(element_2.has_value());
}

TEST(SPSCQueueTests, cancel) {
  SPSCUnboundedBlockingQueue<int> queue;

  EXPECT_TRUE(queue.Put(1));
  queue.Cancel();
  EXPECT_FALSE(queue.Put(2));
  auto element = queue.Take();
  EXPECT_FALSE(element.has_value());
}

TEST(SPSCQueueTests, fifo) {
  SPSCUnboundedBlockingQueue<int> queue;

  EXPECT_TRUE(queue.Put(1));
  EXPECT_TRUE(queue.Put(2));
  EXPECT_TRUE(queue.Put(3));

  auto element_1 = queue.Take();
  auto element_2 = queue.Take();
  auto element_3 = queue.Take();

  ASSERT_TRUE(element_1.has_value());
  ASSERT_TRUE(element_2.has_value());
  ASSERT_TRUE(element_3.has_value());

  EXPECT_EQ(element_1.value(), 1);
  EXPECT_EQ(element_2.value(), 2);
  EXPECT_EQ(element_3.value(), 3);
}

TEST(SPSCQueueTests, concurrent_fifo) {
  SPSCUnboundedBlockingQueue<int> queue;
  std::vector<int> elements{1, 2, 3, 4, 5};

  auto producer = std::thread([&]() mutable {
    for (int element : elements) {
      EXPECT_TRUE(queue.Put(element));
    }

    queue.Close();
  });

  auto element = queue.Take();
  std::vector<int> consumed;

  while (element.has_value()) {
    consumed.emplace_back(element.value());
    element = queue.Take();
  }

  EXPECT_EQ(elements, consumed);

  producer.join();
}

}  // namespace proud_color_sorter::tests