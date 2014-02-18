#ifndef _YOHUB_NETWORK_ASYNC_CONNECTION_H_
#define _YOHUB_NETWORK_ASYNC_CONNECTION_H_

#include "network/inet_address.h"
#include "network/callbacks.h"
#include "network/channel.h"
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

namespace yohub {

class EventPool;

class AsyncConnection : boost::noncopyable {
  public:
    AsyncConnection(EventPool* event_pool,
                    int socket_fd,
                    const InetAddress& local_addr,
                    const InetAddress& peer_addr);
    ~AsyncConnection();

    void Write();
    void Connected();
    void Destroyed();

    void SetConnectionCallback(const ConnectionCallback& callback) {}
    void SetWriteCompletionCallback(const WriteCompletionCallback& callback) {}
    void SetReadCompletionCallback(const ReadCompletionCallback& callback) {}

  private:
    void OnRead();
    void OnWrite();
    void OnClose();

    EventPool* event_pool_;
    int socket_fd_;
    Channel channel_;
    InetAddress local_addr_;
    InetAddress peer_addr_;

    ConnectionCallback on_connection_cb_;
    WriteCompletionCallback on_write_completion_cb_;
    ReadCompletionCallback on_read_completion_cb_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_ASYNC_CONNECTION_H_
