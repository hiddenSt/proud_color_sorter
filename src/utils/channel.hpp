#pragma once

#include <vector>
#include <memory>

#include <spsc_queue.hpp>
#include <color.hpp>

namespace proud_color_sorter::utils {

using Channel = SPSCUnboundedBlockingQueue<std::vector<Color>>;

/// Workaround to be able to cancel channel on signal from user.
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

}