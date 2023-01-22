#include <utils/app.hpp>

#include <csignal>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

#include <fmt/core.h>
#include <fmt/format.h>

#include <counting_sort.hpp>
#include <order.hpp>
#include <utils/channel.hpp>
#include <utils/color_formatter.hpp>
#include <utils/random_generator.hpp>

namespace proud_color_sorter::utils {

namespace detail {

class ThreadExceptionHandle {
 public:
  ThreadExceptionHandle() = default;

  void Set(std::exception_ptr exception) {
    std::lock_guard lock{exception_lock_};
    exception_ = exception;
  }

  [[nodiscard]] bool IsEmpty() {
    std::lock_guard lock{exception_lock_};
    return exception_ == nullptr;
  }

  void Rethrow() {
    std::lock_guard lock{exception_lock_};
    std::rethrow_exception(exception_);
  }

  [[nodiscard]] std::string What() {
    std::lock_guard lock{exception_lock_};
    std::string message;

    try {
      std::rethrow_exception(exception_);
    } catch (const std::exception& error) {
      message = error.what();
    }

    return message;
  }

 private:
  std::mutex exception_lock_;
  std::exception_ptr exception_;
};

std::vector<Color> GenerateColors(RandomGenerator<std::uint64_t>& size_generator,
                                  RandomGenerator<std::uint64_t>& color_generator) {
  auto color_size = size_generator.Generate();
  std::vector<Color> colors;
  colors.reserve(color_size);

  for (std::size_t i = 0; i < color_size; ++i) {
    auto generated = color_generator.Generate();
    colors.emplace_back(static_cast<Color>(generated));
  }

  return colors;
}

void Consume(Channel& channel, const ColorOrder& order) {
  while (true) {
    auto colors = channel.Take();

    if (!colors.has_value()) {
      return;
    }

    fmt::print("Generated colors (size={}): {} \n", colors->size(), fmt::join(colors.value(), " "));
    auto sorted_colors = CountingSort(colors.value(), order);
    fmt::print("Sorted colors (size={}): {} \n", sorted_colors.size(), fmt::join(sorted_colors, " "));
  }
}

void Produce(Channel& channel, const std::size_t max_seq_length) {
  std::vector<Color> colors;
  RandomGenerator<std::uint64_t> size_generator{1, max_seq_length};
  RandomGenerator<std::uint64_t> color_generator{0, kColorSize - 1};

  do {
    colors = detail::GenerateColors(size_generator, color_generator);
  } while (channel.Put(std::move(colors)));
}

void SignalHandler(int signal) {
  if (signal != SIGINT) {
    return;
  }

  ChannelSingleton::Get().GetChannel().Cancel();
}

}  // namespace detail

void RunApp(const Config& config) {
  ColorOrder color_order;

  for (std::size_t i = 0; i < config.color_order.size(); ++i) {
    color_order.Set(config.color_order[i], i);
  }

  Channel channel;
  ChannelSingleton::Get().Set(&channel);
  std::signal(SIGINT, ::proud_color_sorter::utils::detail::SignalHandler);
  detail::ThreadExceptionHandle producer_exception_handle;

  auto producer = std::thread([&channel, config, &producer_exception_handle]() mutable {
    try {
      detail::Produce(channel, config.generated_seq_max_size);
    } catch (const std::exception&) {
      channel.Cancel();
      producer_exception_handle.Set(std::current_exception());
    }
  });

  try {
    detail::Consume(channel, color_order);
  } catch (const std::exception& error) {
    channel.Cancel();
    fmt::print(stderr, "Exception caught from consumer: {}.", error.what());
  }

  producer.join();

  if (!producer_exception_handle.IsEmpty()) {
    fmt::print(stderr, "Exception caught from producer: {}.", producer_exception_handle.What());
  }

  fmt::print("Threads are stopped.\n");
}

}  // namespace proud_color_sorter::utils