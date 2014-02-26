#ifndef _YOHUB_SHARE_CLOCK_H_
#define _YOHUB_SHARE_CLOCK_H_

#include <stdint.h>

namespace yohub {

class Clock {
  public:
    explicit Clock(int64_t now_ms);

    static int64_t NowMicros();

    int64_t now_ms() { return now_ms_; }

  private:
    int64_t now_ms_;

    static const int kMsPerSecond = 1e6;
};

} // namespace yohub

#endif // _YOHUB_SHARE_CLOCK_H_
