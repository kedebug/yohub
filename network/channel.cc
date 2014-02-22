#include "network/channel.h"
#include "network/event_pool.h"
#include <sys/epoll.h>

using namespace yohub;

volatile int Channel::s_sequence_number_ = 0;

Channel::Channel(EventPool* event_pool, int fd)
    : id_(AtomicInc(s_sequence_number_)), 
      fd_(fd),
      events_(EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLET),
      revents_(0),
      status_(0),
      event_pool_(event_pool)
{
}

Channel::~Channel() {
    LOG_TRACE("channel destructor, id=%d, fd=%d", id_, fd_);
}

void Channel::Register() {
    event_pool_->AttachChannel(this);
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

void Channel::EventHandler() {
    int revents = AtomicGetValue(revents_);

    LOG_TRACE("this=%x, id=%d, fd=%d: %s", 
        this, id_, fd_, EventsToString(revents).c_str());

    if ((revents & EPOLLHUP) && !(revents & EPOLLIN)) {
        if (close_callback_)
            close_callback_();
    }
    if (revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
        if (read_callback_) 
            read_callback_();
    }
    if (revents & EPOLLOUT) {
        if (write_callback_)
            write_callback_();
    }
}

std::string Channel::EventsToString(int events) {
    std::string result;
    
    if (events & EPOLLIN)   result += "IN ";
    if (events & EPOLLOUT)  result += "OUT ";
    if (events & EPOLLPRI)  result += "PRI ";
    if (events & EPOLLHUP)  result += "HUP ";
    if (events & EPOLLRDHUP) result += "RDHUP ";
    if (events & EPOLLERR)  result += "ERR ";

    return result;
}
