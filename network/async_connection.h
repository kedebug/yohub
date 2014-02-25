#ifndef _YOHUB_NETWORK_ASYNC_CONNECTION_H_
#define _YOHUB_NETWORK_ASYNC_CONNECTION_H_

#include "network/inet_address.h"
#include "network/socket.h"
#include "network/callbacks.h"
#include "network/channel.h"
#include "network/buffer.h"
#include "share/atomic.h"
#include <string>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace yohub {

class EventPool;

class AsyncConnection : boost::noncopyable,
                        public boost::enable_shared_from_this<AsyncConnection> {
  public:
    AsyncConnection(EventPool* event_pool,
                    int socket_fd,
                    int conn_id,
                    const InetAddress& local_addr,
                    const InetAddress& peer_addr);
    ~AsyncConnection();

    void Write(const std::string& s);
    void Write(const char* data, size_t size);

    void Establish();
    void Destroy();
    void Shutdown();

    void SetConnectionCallback(const ConnectionCallback& callback) {
        on_connection_cb_ = callback;
    }

    void SetWriteCompletionCallback(const WriteCompletionCallback& callback) {
        on_write_completion_cb_ = callback;
    }

    void SetReadCompletionCallback(const ReadCompletionCallback& callback) {
        on_read_completion_cb_ = callback;
    }

    void SetCloseCallback(const CloseCallback& callback) {
        on_close_cb_ = callback;
    }

    const InetAddress& local_addr() { return local_addr_; }
    const InetAddress& peer_addr()  { return peer_addr_; }

    int id() const { return id_; }
    int refs() { return AtomicGetValue(refs_); }
    bool connected() { return AtomicGetValue(is_connected_) == 1; }

  private:
    void QueueWrite(const std::string& s);

    void OnRead();
    void OnWrite();
    void OnClose();

    EventPool* event_pool_;
    Socket socket_;
    Channel channel_;
    InetAddress local_addr_;
    InetAddress peer_addr_;

    const int id_;
    volatile int refs_;
    volatile int is_connected_;

    Buffer in_buffer_;
    Buffer out_buffer_;

    ConnectionCallback on_connection_cb_;
    WriteCompletionCallback on_write_completion_cb_;
    ReadCompletionCallback on_read_completion_cb_;
    CloseCallback on_close_cb_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_ASYNC_CONNECTION_H_
