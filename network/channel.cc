#include "network/channel.h"
#include <sys/epoll.h>

using namespace yohub;

Channel::Channel(IoEvent* io_event, int fd)
    : fd_(fd),
      events_(EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLET),
      revents_(0),
      io_event_(io_event)
{
}

void Channel::Register() {

}

void Channel::Unregister() {
}
    
