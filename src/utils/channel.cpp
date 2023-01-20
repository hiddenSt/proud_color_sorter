#include <memory>
#include <utils/channel.hpp>

namespace proud_color_sorter::utils {

ChannelSingleton* ChannelSingleton::singleton_ = nullptr;

ChannelSingleton& ChannelSingleton::Get() {
  if (singleton_ == nullptr) {
    singleton_ = new ChannelSingleton{};
  }

  return *singleton_;
}

Channel& ChannelSingleton::GetChannel() {
  return *channel_;
}

void ChannelSingleton::Set(Channel* channel) {
  channel_ = channel;
}

ChannelSingleton::~ChannelSingleton() {
  if (singleton_ == nullptr) {
    return;
  }

  delete singleton_;
}

}  // namespace proud_color_sorter::utils