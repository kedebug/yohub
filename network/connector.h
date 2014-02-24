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
    void Connecting(int sockfd);
    void Retry();
    void Disconnect();

    void OnConnect();

  private:
    enum { kDisconnected, kConnecting, kConnected };

    void QueueConnect();

    EventPool* event_pool_;
    InetAddress remote_addr_;
    volatile int connect_;
    volatile int status_;

    boost::scoped_ptr<Channel> conn_channel_;
    NewConnectionCallback on_new_connection_cb_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_CONNECTOR_H_
