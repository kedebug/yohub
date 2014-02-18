#include "network/async_connection.h"
#include "network/event_pool.h"
#include <boost/bind.hpp>

using namespace yohub;

AsyncConnection::AsyncConnection(EventPool* event_pool,
                                 int socket_fd,
                                 const InetAddress& local_addr,
                                 const InetAddress& peer_addr)
    : event_pool_(event_pool),
      socket_fd_(socket_fd),
      channel_(event_pool_, socket_fd_),
      local_addr_(local_addr),
      peer_addr_(peer_addr)
{
    channel_.SetReadCallback(boost::bind(&AsyncConnection::OnRead, this));
    channel_.SetWriteCallback(boost::bind(&AsyncConnection::OnWrite, this));
    channel_.SetCloseCallback(boost::bind(&AsyncConnection::OnClose, this));
}

void AsyncConnection::Write() {
}

void AsyncConnection::Connected() {
    channel_.EnableRead();
    on_connection_cb_(this);
}

void AsyncConnection::OnRead() {

}

void AsyncConnection::OnWrite() {

}

void AsyncConnection::OnClose() {

}
