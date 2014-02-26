#ifndef _YOHUB_SHARE_THREADPOOL_H_
#define _YOHUB_SHARE_THREADPOOL_H_

#include "share/atomic.h"
#include "share/thread.h"
#include "share/mutex.h"
#include "share/queue.h"
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace yohub {

class ThreadPool : boost::noncopyable {
  public:
    typedef boost::function<void()> Job;
    typedef Queue<Job> JobQueue;

    ThreadPool();
    ~ThreadPool();

    void Start(int workers);
    void Stop();

    void Schedule(const Job& job, int which);

    int workers() { 
        return AtomicGetValue(workers_);
    }

  private:
    void WorkHandler(int which);
    void TerminationJob();

    volatile int running_;
    volatile int workers_;
    volatile int job_count_;

    boost::ptr_vector<JobQueue> queues_;
    boost::ptr_vector<Thread> threads_;
};

} // namespace yohub

#endif // _YOHUB_SHARE_THREADPOOL_H_
