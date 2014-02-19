#ifndef _YOHUB_NETWORK_ASYNC_CONNECTION_H_
#define _YOHUB_NETWORK_ASYNC_CONNECTION_H_

#include "network/inet_address.h"
#include "network/socket.h"
#include "network/callbacks.h"
#include "network/channel.h"
#include "network/chain_buffer.h"
#include "share/atomic.h"
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

namespace yohub {

class EventPool;

class AsyncConnection : boost::noncopyable {
  public:
    AsyncConnection(EventPool* event_pool,
                    int socket_fd,
                    int conn_id,
                    const InetAddress& local_addr,
                    const InetAddress& peer_addr);
    ~AsyncConnection();

    void Write();
    void Establish();
    void Destroy();

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

    void Acquire() {
        AtomicInc(refs_);
    }

    void Release() {
        if (AtomicDec(refs_) == 0)
            delete this;
    }
    
    int id() const { return id_; }
    int refs() { return AtomicGetValue(refs_); }

  private:
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

    ChainBuffer in_buffer_;
    ChainBuffer out_buffer_;

    ConnectionCallback on_connection_cb_;
    WriteCompletionCallback on_write_completion_cb_;
    ReadCompletionCallback on_read_completion_cb_;
    CloseCallback on_close_cb_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_ASYNC_CONNECTION_H_
