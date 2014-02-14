#ifndef _YOHUB_SHARE_QUEUE_H_ 
#define _YOHUB_SHARE_QUEUE_H_

#include "share/mutex.h"
#include "share/condvar.h"
#include <vector>

namespace yohub {

template<typename T>
class Queue {
  public:
    Queue() : mu_(), cv_(&mu_) { }

    void Push(T x) {
        MutexLock l(&mu_);
        if (elems_.empty()) {
            cv_.Signal();
        }
        elems_.push_back(x);
    }

    void FetchAll(std::vector<T>& target) {
        MutexLock l(&mu_);
        while (elems_.empty()) {
            cv_.Wait();
        }
        if (!target.empty()) {
            target.clear();
        }
        target.swap(elems_);
    }

  private:
    Mutex mu_;
    CondVar cv_;
    std::vector<T> elems_;
};

} // namespace yohub

#endif // _YOHUB_JOBQUEUE_H_
