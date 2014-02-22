#ifndef _YOHUB_NETWORK_CHANNEL_H_
#define _YOHUB_NETWORK_CHANNEL_H_

#include "share/atomic.h"
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <string>

namespace yohub {

class EventPool;

class Channel : boost::noncopyable {
  public:
    typedef boost::function<void()> CallbackFn;

    Channel(EventPool* event_pool, int fd);
    ~Channel();

    void Register();
    void Unregister();

    void EventHandler();

    void SetStatus(int status);
    void SetReadyEvents(int revents);

    int fd() const { return fd_; }
    uint32_t id() const { return id_; }
    int events() const { return events_; }
    int status() { return AtomicGetValue(status_); }

    void SetReadCallback(const CallbackFn& callback_fn) {
        read_callback_ = callback_fn;
    }

    void SetWriteCallback(const CallbackFn& callback_fn) {
        write_callback_ = callback_fn;
    }

    void SetCloseCallback(const CallbackFn& callback_fn) {
        close_callback_ = callback_fn;
    }

    static std::string EventsToString(int events);

  private:
    static volatile uint32_t s_sequence_number_;

    const uint32_t id_;
    const int fd_;
    const int events_;
    volatile int revents_;
    volatile int status_;
    EventPool* event_pool_;

    CallbackFn read_callback_;
    CallbackFn write_callback_;
    CallbackFn close_callback_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_CHANNEL_H_
