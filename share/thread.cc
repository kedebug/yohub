#include "share/thread.h"
#include "share/utils.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <boost/bind.hpp>

using namespace yohub;

namespace {
    __thread int t_cached_tid = 0;
}

static void CacheTid() {
    if (t_cached_tid == 0) {
        t_cached_tid = static_cast<int>(::syscall(SYS_gettid));
    }
}

Thread::Thread(const BGWorker& worker)
    : started_(false),
      joined_(false),
      pid_(0),
      worker_(worker)
{
}

Thread::~Thread() {
    if (started_ && !joined_) {
        utils::PthreadCall("detach thread", pthread_detach(pid_));
    }
}

namespace {
struct StartThreadState {
    Thread::BGWorker worker;
};
}

static void* BGWorkerWrapper(void* arg) {
    StartThreadState* state = reinterpret_cast<StartThreadState*>(arg);
    state->worker();
    delete state;
    return NULL;
}

void Thread::Start() {
    StartThreadState* state = new StartThreadState;
    state->worker = worker_;

    assert(!started_);
    started_ = true;
    utils::PthreadCall("start thread",
        pthread_create(&pid_, NULL, &BGWorkerWrapper, state));
}

void Thread::Join() {
    assert(started_);
    assert(!joined_);
    joined_ = true;
    utils::PthreadCall("join thread", pthread_join(pid_, NULL));
}

int Thread::SelfId() {
    CacheTid();
    return t_cached_tid; 
}
