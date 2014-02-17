#ifndef _YOHUB_NETWORK_ACCEPTOR_H_
#define _YOHUB_NETWORK_ACCEPTOR_H_

#include "network/channel.h"
#include <boost/noncopyable.hpp>

namespace yohub {

class EventPool;

class Acceptor : boost::noncopyable {
  public:
    Acceptor();
    ~Acceptor();

    void Listen();
    void OnAccept();

  private:
    EventPool* event_pool_;
    Channel accept_channel_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_ACCEPTOR_H_
