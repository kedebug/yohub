#include "network/epoller.h"

using namespace yohub;

EPoller::EPoller()
    : epoll_fd_(::epoll_create1(EPOLL_CLOEXEC)),
      events_(16)
{
}

EPoller::~EPoller() {
    ::close(epoll_fd_);
}

void EPoller::Poll(int timeout_ms, ChannelList* active) {
    int size = ::epoll_wait(
        epoll_fd_, &*events_.begin(), events_.size(), timeout_ms);
    if (size > 0) {

    }
}
