#include "network/acceptor.h"
#include "share/log.h"
#include "share/atomic.h"
#include <boost/bind.hpp>

using namespace yohub;

Acceptor::Acceptor(EventPool* event_pool)
    : bind_(0),
      event_pool_(event_pool),
      accept_socket_(Socket::CreateNonblockingSocket()),
      accept_channel_(event_pool, accept_socket_.fd())
{
    accept_channel_.SetReadCallback(boost::bind(&Acceptor::OnAccept, this));
}

Acceptor::~Acceptor() {
    accept_channel_.DisableAll();
    accept_channel_.Unregister();
}

void Acceptor::SetAndBind(const InetAddress& bindaddr) {
    AtomicSetValue(bind_, 1);

    accept_socket_.SetReuseAddr(true);
    accept_socket_.SetTcpNoDelay(true);
    accept_socket_.Bind(bindaddr);
}

void Acceptor::Listen() {
    assert(AtomicGetValue(bind_));

    accept_socket_.Listen();
    accept_channel_.EnableRead();
}

void Acceptor::OnAccept() {
    InetAddress peeraddr(0);
    int newfd = accept_socket_.Accept(&peeraddr);

    if (newfd >= 0) {
        if (on_new_connection_) {
            on_new_connection_(newfd, peeraddr);
        } else {
            ::close(newfd);
            LOG_WARNING("the new connection callback should set");
        }
    } else {
        LOG_WARNING("OnAccept failed, fd=%d", accept_socket_.fd());
    }
}
