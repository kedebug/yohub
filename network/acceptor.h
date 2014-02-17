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
        void (int newfd, const InetAddress& peeraddr)> CallbackFn;

    Acceptor(EventPool* event_pool);
    ~Acceptor();

    void Listen();
    void OnAccept();

    void SetAndBind(const InetAddress& bindaddr);

    void SetNewConnectionCallback(const CallbackFn& callback_fn) {
        on_new_connection_ = callback_fn;
    }

  private:
    volatile int bind_;
    EventPool* event_pool_;
    Socket accept_socket_; 
    Channel accept_channel_;

    CallbackFn on_new_connection_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_ACCEPTOR_H_
