#include "network/event_pool.h"
#include "network/socket.h"
#include <boost/bind.hpp>
#include <sys/eventfd.h>

using namespace yohub;

namespace {
    const int kPollTimeMs = 10000;
}

EventPool::EventPool(int pollers, int backends) 
    : num_pollers_(pollers),
      num_backends_(backends)
{
}

EventPool::~EventPool() {
    if (AtomicGetValue(running_) == 1) {
        Stop();
    }
}

void EventPool::Run() {
    AtomicSetValue(running_, 1);

    poller_handler_.Start(num_pollers_);
    backend_handler_.Start(num_backends_);

    for (int i = 0; i < num_pollers_; i++) {
        int wakeup_fd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        wakeup_socks_.push_back(new Socket(wakeup_fd));
        wakeup_chans_.push_back(new Channel(this, wakeup_fd));

        pollers_.push_back(new EPoller);
        pollers_.back().AttachChannel(&(wakeup_chans_.back()));

        poller_handler_.Schedule(
            boost::bind(&EventPool::PollWrapper, this, i), i);
    }
}

void EventPool::Stop() {
    if (AtomicSetValue(running_, 0) == 1) {
        WakeUp();
        poller_handler_.Stop();
        backend_handler_.Stop();
    }
}

void EventPool::WakeUp() {
    for (size_t i = 0; i < wakeup_socks_.size(); i++) {
        uint64_t val = 1;
        int result = ::write(wakeup_socks_[i].fd(), &val, sizeof(val));
        if (result != sizeof(val)) {
            LOG_WARN("WakeUp error: %s", strerror(errno));
        }
    }
}

void EventPool::PostJob(const Job& job, int which) {
    backend_handler_.Schedule(job, which);
}

void EventPool::PostJob(const Job& job, const Channel& channel) {
    backend_handler_.Schedule(job, channel.id() % num_backends_);
}

void EventPool::PollWrapper(int which) {
    ChannelList active_channels;
    EPoller& poller = pollers_[which];

    while (AtomicGetValue(running_) == 1) {
        active_channels.clear();
        poller.Poll(kPollTimeMs, &active_channels);

        for (ChannelList::iterator iter = active_channels.begin();
             iter != active_channels.end(); iter++) {
            PostJob(boost::bind(&Channel::EventHandler, *iter), **iter); 
        }
    }
}

void EventPool::AttachChannel(Channel* channel) {
    int which = channel->id() % num_pollers_;
    pollers_[which].AttachChannel(channel);
}

void EventPool::DetachChannel(Channel* channel) {
    int which = channel->id() % num_pollers_;
    pollers_[which].DetachChannel(channel);
}

void EventPool::DisableChannel(Channel* channel) {
    int which = channel->id() % num_pollers_;
    pollers_[which].DisableChannel(channel);
}
