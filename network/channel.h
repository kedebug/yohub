#ifndef _YOHUB_NETWORK_CHANNEL_H_
#define _YOHUB_NETWORK_CHANNEL_H_

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace yohub {

class EventPool;
class Channel : boost::noncopyable {
  public:
    typedef boost::function<void()> CallbackFn;

    Channel(EventPool* event_pool, int fd, int id);

    void Register();
    void Unregister();

    void SetReadyEvents(int revents);
    void EventHandler();

    int fd() const { return fd_; }
    int events() const { return events_; }
    int id() const { return id_; }

  private:
    const int id_;
    const int fd_;
    int events_;
    int revents_;
    EventPool* event_pool_;

    CallbackFn read_callback_;
    CallbackFn read_completion_cb_;
    CallbackFn write_callback_;
    CallbackFn write_completion_cb_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_CHANNEL_H_
