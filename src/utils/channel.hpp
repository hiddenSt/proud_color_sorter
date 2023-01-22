#pragma once

#include <memory>
#include <vector>

#include <color.hpp>
#include <mpsc_queue.hpp>

namespace proud_color_sorter::utils {

using Channel = MPSCUnboundedBlockingQueue<std::vector<Color>>;

/// A workaround to be able to cancel a channel on a signal from the user.
class ChannelSingleton {
 public:
  static ChannelSingleton& Get();

  ~ChannelSingleton();

  Channel& GetChannel();

  void Set(Channel* channel);

 private:
  ChannelSingleton() = default;

 private:
  static ChannelSingleton* singleton_;

  Channel* channel_ = nullptr;
};

}  // namespace proud_color_sorter::utils