#include "share/clock.h"
#include "network/channel.h"
#include "network/event_pool.h"

#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <vector>

#include <sys/socket.h>
#include <sys/resource.h>

using namespace yohub;

int count, writes, fired;
int num_pipes, num_active, num_writes;

std::vector<int> pipes;
boost::ptr_vector<Channel> chans;
EventPool* event_pool;

Mutex mu;
CondVar cv(&mu);

void ReadCallback(int fd, int idx) {
    char ch;
    count += ::recv(fd, &ch, sizeof(ch), 0);
    if (writes > 0) {
        int widx = idx + 1;
        if (widx >= num_pipes) {
            widx -= num_pipes;
        }
        ::send(pipes[2 * widx + 1], "m", 1, 0);
        writes--;
        fired++;
    }
    if (fired == count) {
        event_pool->Stop();
        cv.Signal();
    }
}

void RunOnce() {
    int64_t before = Clock::NowMicros();

    for (int i = 0; i < num_pipes; i++) {
        Channel& chan = chans[i];
        chan.SetReadCallback(
            boost::bind(ReadCallback, chan.fd(), i));
        chan.Register();
    }

    int space = num_pipes / num_active;
    space *= 2;
    for (int i = 0; i < num_active; i++) {
        ::send(pipes[i * space + 1], "m", 1, 0);
    }

    fired = num_active;
    count = 0;
    writes = num_writes;

    int64_t start = Clock::NowMicros();

    event_pool->Run();
    cv.Wait(); 

    int64_t end = Clock::NowMicros(); 

    fprintf(stdout, "%8ld %8ld\n", end - before, end - start);
}

int main(int argc, char* argv[]) {
    log::SetLogLevel(log::FATAL);

    num_pipes = 100;
    num_active = 1;
    num_writes = 100;

    int c;
    while ((c = getopt(argc, argv, "n:a:w:")) != -1) {
        switch (c) {
          case 'n':
            num_pipes = atoi(optarg);
            break;
          case 'a':
            num_active = atoi(optarg);
            break;
          case 'w':
            num_writes = atoi(optarg);
            break;
          default:
            fprintf(stderr, "Illegal argument `%c`\n", c);
            return 1;
        }
    }

    struct rlimit rl;
    rl.rlim_cur = rl.rlim_max = num_pipes * 2 + 50;
    if (::setrlimit(RLIMIT_NOFILE, &rl) == -1) {
        fprintf(stderr, "setrlimit error: %s", strerror(errno));
        return 1;
    }

    pipes.resize(2 * num_pipes);
    for (int i = 0; i < num_pipes; i++) {
        if (::socketpair(AF_UNIX, SOCK_STREAM, 0, &pipes[i * 2]) == -1) {
            fprintf(stderr, "socketpair error: %s", strerror(errno));
            return 1;
        }
    }

    EventPool pool(1, 1);
    event_pool = &pool;

    for (int i = 0; i < num_pipes; i++) {
        chans.push_back(new Channel(event_pool, pipes[i * 2]));
    }

    for (int i = 0; i < 25; i++) {
        RunOnce();
        fprintf(stdout, "writes=%d, fired=%d, recv=%d\n", writes, fired, count);
    }
}
