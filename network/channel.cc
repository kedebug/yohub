#include "network/channel.h"
#include <sys/epoll.h>

using namespace yohub;

Channel::Channel(EventPool* event_pool, int fd, int id)
    : id_(0), 
      fd_(fd),
      events_(EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLET),
      revents_(0),
      event_pool_(event_pool)
{
}

void Channel::Register() {

}

void Channel::Unregister() {
}
    
