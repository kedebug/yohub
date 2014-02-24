#include "network/connector.h"
#include "network/event_pool.h"
#include "network/socket.h"
#include <boost/bind.hpp>

using namespace yohub;

Connector::Connector(EventPool* event_pool,
                     const InetAddress& remote_addr)
    : event_pool_(event_pool),
      remote_addr_(remote_addr),
      connect_(0),
      status_(kDisconnected)
{
}

Connector::~Connector() {

}

void Connector::Connect() {
    if (AtomicSetValue(connect_, 1) == 0) {
        event_pool_->PostJob(boost::bind(&Connector::QueueConnect, this));
    }
}

void Connector::QueueConnect() {
    int newfd = Socket::CreateNonblockingSocket();
    int result = Socket::Connect(newfd, remote_addr_.sockaddr_in());
    if (result == 0 || errno == EINPROGRESS) {
        Connecting(newfd);
    } else {
        ::close(newfd);
        LOG_WARN("connect error: %s", strerror(errno));
    }
}

void Connector::Connecting(int sockfd) {
    AtomicSetValue(status_, kConnecting);

    conn_channel_.reset(new Channel(event_pool_, sockfd));
    conn_channel_->SetWriteCallback(
        boost::bind(&Connector::OnConnect, this));

    conn_channel_->Register();
}

void Connector::OnConnect() {
    if (AtomicSetValue(status_, kConnected) == kConnecting) {
        conn_channel_->Unregister();
        int fd = conn_channel_->fd();
        if (AtomicGetValue(connect_) == 1) 
            on_new_connection_cb_(fd);
        else
            ::close(fd);
    } else {
        LOG_WARN("OnConnect error occur");
    }
}

void Connector::Retry() {

}

void Connector::Disconnect() {

}
