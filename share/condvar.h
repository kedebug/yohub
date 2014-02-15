#ifndef _YOHUB_SHARE_CONDVAR_H_
#define _YOHUB_SHARE_CONDVAR_H_

#include <boost/noncopyable.hpp>
#include "share/mutex.h"

namespace yohub {

class CondVar : boost::noncopyable {
  public:
    explicit CondVar(Mutex* mutex) 
        : mutex_(mutex) {
        utils::PthreadCall("init cv", pthread_cond_init(&cv_, NULL));
    }

    ~CondVar() {
        utils::PthreadCall("destroy cv", pthread_cond_destroy(&cv_));
    }

    void Wait() {
        utils::PthreadCall("wait", pthread_cond_wait(&cv_, &mutex_->mtx_));
    }

    bool TimedWait(int seconds) {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += seconds;
        return ETIMEDOUT == pthread_cond_timedwait(&cv_, &mutex_->mtx_, &abstime);
    }

    void Signal() {
        utils::PthreadCall("signal", pthread_cond_signal(&cv_)); 
    }

    void SignalAll() {
        utils::PthreadCall("broadcast", pthread_cond_broadcast(&cv_));
    }

  private:
    pthread_cond_t cv_;
    Mutex* mutex_;
};

} // namespace yohub

#endif // _YOHUB_SHARE_CONDVAR_H_
