#include "share/atomic.h"
#include "network/connector.h"
#include "network/async_client.h"
#include "network/socket.h"
#include "network/async_connection.h"
#include <boost/bind.hpp>

using namespace yohub;

AsyncClient::AsyncClient(EventPool* event_pool, 
                         const InetAddress& remote)
    : event_pool_(event_pool),
      connector_(event_pool_, remote),
      connect_(0)
{
    connector_.SetNewConnectionCallback(
        boost::bind(&AsyncClient::OnNewConnection, this, _1));
}

AsyncClient::~AsyncClient() {
    MutexLock l(&mu_);
    if (client_conn_) {
        client_conn_->Destroy();
    }
}

void AsyncClient::Connect() {
    AtomicSetValue(connect_, 1);
    connector_.Connect();
}

void AsyncClient::Disconnect() {
    AtomicSetValue(connect_, 0);

    MutexLock l(&mu_);
    if (client_conn_) {
        client_conn_->Shutdown();
    }
}

void AsyncClient::OnNewConnection(int sockfd) {
    InetAddress local(Socket::GetLocalSockAddr(sockfd));
    InetAddress remote(Socket::GetPeerSockAddr(sockfd));

    AsyncConnectionPtr new_conn(
        new AsyncConnection(event_pool_, sockfd, 0, local, remote));

    new_conn->SetConnectionCallback(on_connection_cb_);
    new_conn->SetReadCompletionCallback(on_read_completion_cb_);
    new_conn->SetWriteCompletionCallback(on_write_completion_cb_);
    new_conn->SetCloseCallback(
        boost::bind(&AsyncClient::OnCloseConnection, this, _1));

    new_conn->Establish();

    MutexLock l(&mu_);
    client_conn_ = new_conn;
}

void AsyncClient::OnCloseConnection(const AsyncConnectionPtr& conn) {
    mu_.Lock();
    assert(client_conn_ == conn);
    client_conn_.reset();
    mu_.Unlock();

    conn->Destroy();
}
