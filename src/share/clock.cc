#include "share/clock.h"
#include <stdio.h>
#include <sys/time.h>

using namespace yohub;

Clock::Clock(int64_t now_ms)
    : now_ms_(now_ms)
{
}

int64_t Clock::NowMicros() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t sec = tv.tv_sec;
    return sec * kMsPerSecond + tv.tv_usec;
}
