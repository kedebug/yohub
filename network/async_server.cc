#include "share/atomic.h"
#include "share/log.h"
#include "network/async_server.h"
#include <boost/bind.hpp>

using namespace yohub;

AsyncServer::AsyncServer(EventPool* event_pool, 
                         const InetAddress& bindaddr)
    : event_pool_(event_pool),
      started_(0),
      num_connections_(0),
      acceptor_(event_pool_),
      bind_addr_(bindaddr)
{
    acceptor_.SetNewConnectionCallback(
        boost::bind(&AsyncServer::OnNewConnection, this, _1, _2));
}

AsyncServer::~AsyncServer() {
}

void AsyncServer::Start() {
    if (AtomicSetValue(started_, 1) == 0) {
        acceptor_.SetAndBind(bind_addr_);
        acceptor_.Listen();
        LOG_TRACE("server started, ip:port=%s:%u", 
            bind_addr_.ip().c_str(), bind_addr_.port());
    }
}

void AsyncServer::OnNewConnection(int sockfd, 
                                  const InetAddress& peeraddr) {
    InetAddress local_addr(Socket::GetSocketName(sockfd));
    AsyncConnection* new_conn = new 
        AsyncConnection(event_pool_, sockfd, local_addr, peeraddr);
    
    new_conn->SetConnectionCallback(on_connection_cb_);
    new_conn->SetWriteCompletionCallback(on_write_completion_cb_);
    new_conn->SetReadCompletionCallback(on_read_completion_cb_);

    connections_[AtomicInc(num_connections_)] = new_conn;
    new_conn->Establish();
}
