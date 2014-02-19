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

    void EnableRead(); 
    void EnableWrite();
    void DisableWrite();

    void DisableAll();

    bool WriteAllowed();

    int fd() const { return fd_; }
    int id() const { return id_; }
    int status() { return AtomicGetValue(status_); }
    int events() { return AtomicGetValue(events_); }

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
    void Update();

    static volatile int s_sequence_number_;

    const int id_;
    const int fd_;
    volatile int events_;
    volatile int revents_;
    volatile int status_;
    EventPool* event_pool_;

    CallbackFn read_callback_;
    CallbackFn write_callback_;
    CallbackFn close_callback_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_CHANNEL_H_
