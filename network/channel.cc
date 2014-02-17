#include "network/channel.h"
#include "network/event_pool.h"
#include <sys/epoll.h>

using namespace yohub;

volatile int Channel::s_sequence_number_ = 0;

Channel::Channel(EventPool* event_pool, int fd)
    : id_(AtomicInc(s_sequence_number_)), 
      fd_(fd),
      events_(0),
      revents_(0),
      status_(0),
      event_pool_(event_pool)
{
}

Channel::~Channel() {
}

void Channel::Register() {
    Update();
}

void Channel::Unregister() {
    event_pool_->DetachChannel(this);
}

void Channel::SetStatus(int status) {
    AtomicSetValue(status_, status);
}

void Channel::SetReadyEvents(int revents) {
    AtomicSetValue(revents_, revents);
}

void Channel::EnableRead() {
    AtomicSetValue(events_, events() | EPOLLIN | EPOLLPRI);
    Update();
}

void Channel::EnableWrite() {
    AtomicSetValue(events_, events() | EPOLLOUT);
    Update();
}

void Channel::DisableWrite() {
    AtomicSetValue(events_, events() & (~EPOLLOUT));
    Update();
}

void Channel::DisableAll() {
    AtomicSetValue(events_, 0);
    Update();
}

bool Channel::WriteAllowed() {
    return AtomicGetValue(events_) & EPOLLOUT;
}

void Channel::Update() {
    event_pool_->AttachChannel(this);
}
