#ifndef _YOHUB_SHARE_THREADPOOL_H_
#define _YOHUB_SHARE_THREADPOOL_H_

#include "share/async.h"
#include "share/atomic.h"
#include "share/thread.h"
#include "share/queue.h"
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace yohub {

class ThreadPool : boost::noncopyable {
  public:
    typedef Queue<Job*> JobQueue;
    typedef boost::ptr_vector<JobQueue>::auto_type QueuePtr;

    ThreadPool();
    ~ThreadPool();

    void Start(int workers);
    void Stop();

    void ScheduleJob(Job* job, int which);
    void CancelJob(Job* job, int which);

    int workers() const { 
        return AtomicGetValue(workers_);
    }

  private:
    void WorkHandler(int which);

    volatile int running_;
    volatile int workers_;
    boost::ptr_vector<JobQueue> queues_;
    boost::ptr_vector<Thread> threads_;
};

} // namespace yohub

#endif // _YOHUB_SHARE_THREADPOOL_H_
