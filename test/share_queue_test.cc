#include "share/thread.h"
#include "share/queue.h"
#include "share/atomic.h"
#include <stdio.h>
#include <vector>

using namespace yohub;

Queue<int> g_queue;
volatile int g_flag;
volatile int g_count;

void FetchProc() {
    int sum = 0;
    int times = 0;
    while (g_flag) {
        std::vector<int> take;
        g_queue.FetchAll(&take, 1);
        times++;
        sum += take.size();
    }
    printf("sum = %d, times = %d\n", sum, times);
}

void PutProc() {
    while (g_flag) {
        g_queue.Push(1);
        AtomicInc(g_count);
    }
}

int main() {
    g_flag = 1;
    g_count = 0;

    Thread thr1(FetchProc);
    Thread thr2(PutProc);
    Thread thr3(PutProc);

    thr1.Start();
    thr2.Start();
    thr3.Start();

    sleep(2);
    AtomicSetValue(g_flag, 0);

    thr1.Join();
    thr2.Join();
    thr3.Join();

    printf("put num: %d\n", g_count);
}
