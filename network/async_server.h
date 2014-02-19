#ifndef _YOHUB_NETWORK_ASYNC_SERVER_H_
#define _YOHUB_NETWORK_ASYNC_SERVER_H_

#include "network/event_pool.h"
#include "network/async_connection.h"
#include "network/acceptor.h"
#include "network/callbacks.h"
#include <map>
#include <boost/noncopyable.hpp>

namespace yohub {

class AsyncServer : boost::noncopyable {
  public:
    typedef std::map<int, AsyncConnection*> ConnectionMap;

    AsyncServer(EventPool* event_pool, const InetAddress& bindaddr);
    ~AsyncServer();

    void Start();

    void SetConnectionCallback(const ConnectionCallback& callback) {
        on_connection_cb_ = callback;
    }

    void SetWriteCompletionCallback(const WriteCompletionCallback& callback) {
        on_write_completion_cb_ = callback;
    }

    void SetReadCompletionCallback(const ReadCompletionCallback& callback) {
        on_read_completion_cb_ = callback;
    }

  private:
    // our acceptor only runs on a fixed thread.
    void OnNewConnection(int sockfd, const InetAddress& peeraddr);

    EventPool* event_pool_;
    volatile int started_;
    volatile int num_connections_;
    ConnectionMap connections_;
    Acceptor acceptor_;
    InetAddress bind_addr_;

    ConnectionCallback on_connection_cb_;
    WriteCompletionCallback on_write_completion_cb_;
    ReadCompletionCallback on_read_completion_cb_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_ASYNC_SERVER_H_
