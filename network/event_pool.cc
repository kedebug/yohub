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
}

void EventPool::Run() {
    poller_handler_.Start(num_pollers_);
    backend_handler_.Start(num_backends_);

    for (int i = 0; i < num_pollers_; i++) {
        pollers_.push_back(new EPoller);
        poller_handler_.Schedule(
            boost::bind(&EventPool::PollWrapper, this, i), i);
    }
}

void EventPool::Stop() {
}

void EventPool::PollWrapper(int which) {
    ChannelList active_channels;
    EPoller& poller = pollers_[which];

    while (running_) {
        active_channels.clear();
        poller.Poll(kPollTimeMs, &active_channels);

        for (size_t i = 0; i < active_channels.size(); i++) {
            backend_handler_.Schedule(
                boost::bind(&Channel::EventHandler, active_channels[i]),
                active_channels[i]->id() % num_backends_);
        }
    }
}
