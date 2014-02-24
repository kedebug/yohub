#ifndef _YOHUB_NETWORK_CONNECTOR_H_
#define _YOHUB_NETWORK_CONNECTOR_H_

#include "network/inet_address.h"
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace yohub {

class Channel;
class EventPool;

class Connector : boost::noncopyable {
  public:
    typedef boost::function<void (int sockfd)> NewConnectionCallback;

    Connector(EventPool* event_pool, const InetAddress& remote);
    ~Connector();

    void Connect();
    void Retry();

    void SetNewConnectionCallback(const NewConnectionCallback& cb) {
        on_new_connection_cb_ = cb;
    }

  private:
    enum { kDisconnected, kConnecting, kConnected };

    void OnConnect();
    void QueueConnect();
    void Connecting(int sockfd);

    EventPool* event_pool_;
    InetAddress remote_addr_;
    volatile int connect_;
    volatile int status_;

    boost::scoped_ptr<Channel> conn_channel_;
    NewConnectionCallback on_new_connection_cb_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_CONNECTOR_H_
