#ifndef _YOHUB_NETWORK_IOEVENT_H_
#define _YOHUB_NETWORK_IOEVENT_H_

#include "network/epoller.h"
#include "network/channel.h"
#include <boost/noncopyable.hpp>

namespace yohub {

class IoEvent : boost::noncopyable {
  public:
    IoEvent();
    ~IoEvent();

    void RunLoop();
    void Stop();

    void AttachChannel(Channel* channel);
    void DetachChannel(Channel* channel);

  private:
    void RunLoopOnce();
    
    volatile int running_;

    EPoller* epoller_;
    EPoller::ChannelList active_channels_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_IOEVENT_H_
