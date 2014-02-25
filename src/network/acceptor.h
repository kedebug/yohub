#ifndef _YOHUB_NETWORK_ACCEPTOR_H_
#define _YOHUB_NETWORK_ACCEPTOR_H_

#include "network/channel.h"
#include "network/socket.h"
#include "network/inet_address.h"
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

namespace yohub {

class EventPool;

class Acceptor : boost::noncopyable {
  public:
    typedef boost::function<
        void (int newfd, const InetAddress& peeraddr)> NewConnectionCallback;

    Acceptor(EventPool* event_pool);
    ~Acceptor();

    void Listen();
    void OnAccept();

    void SetAndBind(const InetAddress& bindaddr);

    void SetNewConnectionCallback(const NewConnectionCallback& callback) {
        on_new_connection_cb_ = callback;
    }

  private:
    volatile int bind_;
    EventPool* event_pool_;
    Socket accept_socket_; 
    Channel accept_channel_;
    int idle_fd_;

    NewConnectionCallback on_new_connection_cb_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_ACCEPTOR_H_
