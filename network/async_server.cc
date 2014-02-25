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
    ConnectionMap connections;
    {
        MutexLock l(&mu_);
        connections.swap(connections_);
    }
    for (ConnectionMap::iterator iter = connections.begin();
         iter != connections.end(); iter++) {
        iter->second->Destroy();
        iter->second.reset();
    }
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
                                  const InetAddress& peer) {
    InetAddress local(Socket::GetSocketName(sockfd));
    AsyncConnectionPtr new_conn(new AsyncConnection(
        event_pool_, sockfd, AtomicInc(num_connections_), local, peer));

    new_conn->SetConnectionCallback(on_connection_cb_);
    new_conn->SetWriteCompletionCallback(on_write_completion_cb_);
    new_conn->SetReadCompletionCallback(on_read_completion_cb_);
    new_conn->SetCloseCallback(
        boost::bind(&AsyncServer::OnCloseConnection, this, _1));

    new_conn->Establish();

    MutexLock l(&mu_);
    connections_[new_conn->id()] = new_conn;
}

void AsyncServer::OnCloseConnection(const AsyncConnectionPtr& conn) {
    {
        MutexLock l(&mu_);
        ConnectionMap::iterator iter = connections_.find(conn->id());
        assert(iter != connections_.end());
        connections_.erase(iter);
    }
    conn->Destroy();
}
