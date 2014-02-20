#ifndef _YOHUB_NETWORK_EVENTPOOL_H_
#define _YOHUB_NETWORK_EVENTPOOL_H_

#include "network/epoller.h"
#include "network/channel.h"
#include "share/threadpool.h"
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace yohub {

class EventPool : boost::noncopyable {
  public:
    typedef ThreadPool::Job Job;
    typedef EPoller::ChannelList ChannelList;

    EventPool(int pollers, int backends);
    ~EventPool();

    void Run();
    void Stop();

    void AttachChannel(Channel* channel);
    void DetachChannel(Channel* channel);

    void PostJob(const Job& job, const Channel& channel);

    void PollWrapper(int which);

  private:
    volatile int running_;
    const int num_pollers_;
    const int num_backends_;

    boost::ptr_vector<EPoller> pollers_;
    ThreadPool poller_handler_;
    ThreadPool backend_handler_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_EVENTPOOL_H_
