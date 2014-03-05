#include "share/log.h"
#include "share/atomic.h"
#include "share/threadpool.h"

#include <signal.h>
#include <boost/bind.hpp>

using namespace yohub;

int stop = 0;
int num_gets = 0;
int num_posts = 0;
ThreadPool thread_pool;

void SignalStop(int) {
    AtomicSetValue(stop, 1);
    thread_pool.Stop();
    LOG_TRACE("Actually: %d, expected: %d", num_gets, num_posts);
}

void Get() {
    AtomicInc(num_gets);
}

void Post() {
    while (AtomicGetValue(stop) == 0) {
        if (thread_pool.Schedule(boost::bind(&Get))) {
            AtomicInc(num_posts);
        }
    }
}

int main() {
    ::signal(SIGINT, SignalStop);

    thread_pool.Start(4);

    Thread thr1(boost::bind(&Post));
    Thread thr2(boost::bind(&Post));

    thr1.Start();
    thr2.Start();

    thr1.Join();
    thr2.Join();

    LOG_TRACE("Process exit successfully.");
}
