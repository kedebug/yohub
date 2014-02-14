#include "share/atomic.h"
#include "share/thread.h"
#include "share/log.h"

using namespace yohub;

volatile bool flag;
volatile int count;

void atomic_test() {
    while (flag) {
        AtomicInc(count);
        AtomicDec(count);
    }
}

int main() {
    count = 0;
    flag = true;

    Thread t1(atomic_test);
    Thread t2(atomic_test);
    Thread t3(atomic_test);

    t1.Start();
    t2.Start();
    t3.Start();

    ::sleep(2);
    flag = false;

    t1.Join();
    t2.Join();
    t3.Join();

    LOG_TRACE("count=%d, expected 0", count);
}
