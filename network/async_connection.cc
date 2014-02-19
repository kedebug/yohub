#include "network/async_connection.h"
#include "network/event_pool.h"
#include <boost/bind.hpp>

using namespace yohub;

AsyncConnection::AsyncConnection(EventPool* event_pool,
                                 int socket_fd,
                                 int conn_id,
                                 const InetAddress& local_addr,
                                 const InetAddress& peer_addr)
    : event_pool_(event_pool),
      socket_(socket_fd),
      channel_(event_pool_, socket_fd),
      local_addr_(local_addr),
      peer_addr_(peer_addr),
      id_(conn_id),
      refs_(0),
      is_connected_(0)
{
    channel_.SetReadCallback(boost::bind(&AsyncConnection::OnRead, this));
    channel_.SetWriteCallback(boost::bind(&AsyncConnection::OnWrite, this));
    channel_.SetCloseCallback(boost::bind(&AsyncConnection::OnClose, this));
}

AsyncConnection::~AsyncConnection() {
}

void AsyncConnection::Write() {
}

void AsyncConnection::Establish() {
    if (AtomicSetValue(is_connected_, 1) == 0) {
        channel_.EnableRead();
        on_connection_cb_(this);
    } else {
        LOG_WARN("Connection already established");
    }
}

void AsyncConnection::Destroy() {
    if (AtomicSetValue(is_connected_, 0) == 1) {
        channel_.DisableAll();
        channel_.Unregister();
    } else {
        LOG_WARN("Connection already destroyed");
    }
}

void AsyncConnection::OnRead() {

}

void AsyncConnection::OnWrite() {

}

void AsyncConnection::OnClose() {

}
