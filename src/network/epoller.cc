#include "share/log.h"
#include "network/epoller.h"
#include "network/channel.h"
#include <unistd.h>

using namespace yohub;

namespace {
    const int kNew = 0;
    const int kAdded = 1;
    const int kDeleted = 2;
}

EPoller::EPoller()
    : epoll_fd_(::epoll_create1(EPOLL_CLOEXEC)),
      events_(1024)
{
}

EPoller::~EPoller() {
    ::close(epoll_fd_);
}

void EPoller::Poll(int timeout_ms, ChannelList* active_channels) {
    int num_events = ::epoll_wait(
        epoll_fd_, &*events_.begin(), events_.size(), timeout_ms);

    if (num_events > 0) {
        active_channels->reserve(num_events);
        for (int i = 0; i < num_events; i++) {
            Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
            active_channels->push_back(channel);
            channel->SetReadyEvents(events_[i].events);
        }
        if (num_events == static_cast<int>(events_.size())) {
            events_.resize(num_events << 1);
        }
    } else if (num_events == 0) {
        LOG_TRACE("nothing happened in epoll_wait");
    }
}

void EPoller::AttachChannel(Channel* channel) {
    struct epoll_event ev;

    memset(&ev, 0, sizeof(ev));
    ev.events = channel->events();
    ev.data.ptr = channel;

    channel->SetStatus(kAdded);

    if (::epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, channel->fd(), &ev) < 0) {
        LOG_WARN("epoll_ctl_add error: %s", strerror(errno));
    }
}

void EPoller::DetachChannel(Channel* channel) {
    channel->SetStatus(kDeleted);

    if (::epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, channel->fd(), NULL) < 0) {
        LOG_WARN("epoll_ctl_del error: %s", strerror(errno));
    }
}

void EPoller::DisableChannel(Channel* channel) {
    if (channel->status() == kAdded) {
        struct epoll_event ev;

        memset(&ev, 0, sizeof(ev));
        ev.events = 0;
        ev.data.ptr = channel;

        if (::epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, channel->fd(), &ev) < 0) {
            LOG_WARN("epoll_ctl_mod error: %s", strerror(errno));
        }
    }
}
