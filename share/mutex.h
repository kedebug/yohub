#ifndef _YOHUB_SHARE_MUTEX_H_
#define _YOHUB_SHARE_MUTEX_H_

#include <pthread.h>
#include <boost/noncopyable.hpp>
#include "share/utils.h"

namespace yohub {

class CondVar;

class Mutex : boost::noncopyable {
  public:
    Mutex() {
        utils::PthreadCall("init mutex", pthread_mutex_init(&mtx_, NULL));
    }

    ~Mutex() {
        utils::PthreadCall("destroy mutex", pthread_mutex_destroy(&mtx_)); 
    }

    void Lock() {
        utils::PthreadCall("mutex lock", pthread_mutex_lock(&mtx_));
    }

    void Unlock() {
        utils::PthreadCall("mutex unlock", pthread_mutex_unlock(&mtx_));
    }

    void AssertHeld() {}
    
  private:
    friend class CondVar;
    pthread_mutex_t mtx_;
};

class MutexLock : boost::noncopyable {
  public:
    explicit MutexLock(Mutex* mu)
        : mutex_(mu) {
        this->mutex_->Lock();
    }

    ~MutexLock() {
        this->mutex_->Unlock();
    }

  private:
    Mutex *const mutex_;
};

} // namespace yohub

#endif // _YOHUB_SHARE_MUTEX_H_
