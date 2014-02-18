#include "share/atomic.h"
#include "share/log.h"
#include "network/async_server.h"
#include <boost/bind.hpp>

using namespace yohub;

AsyncServer::AsyncServer(EventPool* event_pool, const InetAddress& bindaddr)
    : event_pool_(event_pool),
      started_(0),
      num_connections_(0),
      acceptor_(event_pool_),
      local_addr_(bindaddr)
{
    acceptor_.SetNewConnectionCallback(
        boost::bind(&AsyncServer::OnNewConnection, this, _1, _2));
}

AsyncServer::~AsyncServer() {
}

void AsyncServer::Start() {
    if (AtomicSetValue(started_, 1) == 0) {
        acceptor_.SetAndBind(local_addr_);
        acceptor_.Listen();
        LOG_TRACE("server started, ip:port=%s:%u", 
            local_addr_.ip().c_str(), local_addr_.port());
    }
}

void AsyncServer::OnNewConnection(int sockfd, const InetAddress& peeraddr) {
    AtomicInc(num_connections_); 
}
