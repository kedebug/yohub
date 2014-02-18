#ifndef _YOHUB_NETWORK_ASYNC_CONNECTION_H_
#define _YOHUB_NETWORK_ASYNC_CONNECTION_H_

#include "network/inet_address.h"
#include "network/callbacks.h"
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

namespace yohub {

class Channel;
class EventPool;

class AsyncConnection : boost::noncopyable {
  public:
    AsyncConnection(EventPool* event_pool,
                    int socket_fd,
                    const InetAddress& local_addr,
                    const InetAddress& peer_addr);
    ~AsyncConnection();

    void Write();

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
    WriteCompleteCallback on_write_complete_cb_;
    ReadCompleteCallback on_read_complete_cb_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_ASYNC_CONNECTION_H_
