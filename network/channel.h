#ifndef _YOHUB_NETWORK_CHANNEL_H_
#define _YOHUB_NETWORK_CHANNEL_H_

#include "share/atomic.h"
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace yohub {

class EventPool;
class Channel : boost::noncopyable {
  public:
    typedef boost::function<void()> CallbackFn;

    Channel(EventPool* event_pool, int fd, int id);
    ~Channel();

    void Register();
    void Unregister();

    void SetStatus(int status);
    void SetReadyEvents(int revents);
    void EventHandler();

    void EnableRead(); 
    void EnableWrite();
    void DisableWrite();

    bool WriteFlagOn();

    int fd() const { return fd_; }
    int id() const { return id_; }
    int status() { return AtomicGetValue(status_); }
    int events() { return AtomicGetValue(events_); }

  private:
    void Update();

    const int id_;
    const int fd_;
    volatile int events_;
    volatile int revents_;
    volatile int status_;
    EventPool* event_pool_;

    CallbackFn read_callback_;
    CallbackFn read_completion_cb_;
    CallbackFn write_callback_;
    CallbackFn write_completion_cb_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_CHANNEL_H_
