#ifndef _YOHUB_NETWORK_CHANNEL_H_
#define _YOHUB_NETWORK_CHANNEL_H_

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace yohub {

class IoEvent;
class Channel : boost::noncopyable {
  public:
    typedef boost::function<void()> CallbackFn;

    Channel(IoEvent* io_event, int fd);

    void Register();
    void Unregister();

    void SetReadyEvents(int revents);

    int fd() const { return fd_; }
    int events() const { return events_; }

  private:
    const int fd_;
    int events_;
    int revents_;
    IoEvent* io_event_;

    CallbackFn read_callback_;
    CallbackFn read_completion_cb_;
    CallbackFn write_callback_;
    CallbackFn write_completion_cb_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_CHANNEL_H_
