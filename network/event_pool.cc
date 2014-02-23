#include "network/event_pool.h"
#include <boost/bind.hpp>

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
        pollers_.push_back(new EPoller);
        poller_handler_.Schedule(
            boost::bind(&EventPool::PollWrapper, this, i), i);
    }
}

void EventPool::Stop() {
    if (AtomicSetValue(running_, 0) == 1) {
        poller_handler_.Stop();
        backend_handler_.Stop();
    }
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

        for (size_t i = 0; i < active_channels.size(); i++) {
            backend_handler_.Schedule(
                boost::bind(&Channel::EventHandler, active_channels[i]),
                active_channels[i]->id() % num_backends_);
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
