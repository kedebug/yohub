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

    void SetConnectionCallback(const ConnectionCallback& callback);
    void SetWriteCompleteCallback(const WriteCompleteCallback& callback);
    void SetReadCompleteCallback(const ReadCompleteCallback& callback);

  private:
    // thread-safe:
    // our acceptor only runs on a fixed thread.
    void OnNewConnection(int sockfd, const InetAddress& peeraddr);

    EventPool* event_pool_;
    volatile int started_;
    volatile int num_connections_;
    ConnectionMap connections_;
    Acceptor acceptor_;
    InetAddress bind_addr_;

    ConnectionCallback on_connection_cb_;
    WriteCompleteCallback on_write_complete_cb_;
    ReadCompleteCallback on_read_complete_cb_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_ASYNC_SERVER_H_
