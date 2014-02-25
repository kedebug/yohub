#ifndef _YOHUB_NETWORK_ASYNCCLIENT_H_
#define _YOHUB_NETWORK_ASYNCCLIENT_H_

#include "network/connector.h"
#include "network/inet_address.h"
#include "network/callbacks.h"
#include "share/mutex.h"
#include <boost/noncopyable.hpp>

namespace yohub {

class Connector;
class EventPool;

class AsyncClient : boost::noncopyable {
  public:
    AsyncClient(EventPool* event_pool, const InetAddress& remote);
    ~AsyncClient();
    
    void Connect();
    void Disconnect();

    void SetConnectionCallback(const ConnectionCallback& cb) {
        on_connection_cb_ = cb;
    }
    void SetReadCompletionCallback(const ReadCompletionCallback& cb) {
        on_read_completion_cb_ = cb;
    }
    void SetWriteCompletionCallback(const WriteCompletionCallback& cb) {
        on_write_completion_cb_ = cb;
    }

  private:
    void OnNewConnection(int sockfd);
    void OnCloseConnection(const AsyncConnectionPtr& conn);

    EventPool* const event_pool_;
    Connector connector_;
    volatile int connect_;

    Mutex mu_;
    AsyncConnectionPtr client_conn_;

    ConnectionCallback on_connection_cb_;
    WriteCompletionCallback on_write_completion_cb_;
    ReadCompletionCallback on_read_completion_cb_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_ASYNCCLIENT_H_
