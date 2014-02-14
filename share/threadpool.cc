#include "share/threadpool.h"
#include <vector>
#include <boost/bind.hpp>

using namespace yohub;

ThreadPool::ThreadPool()
    : running_(0),
      workers_(0)
{
}

ThreadPool::~ThreadPool() {
    if (AtomicGetValue(running_)) {
        Stop();
    }
}

void ThreadPool::Start(int workers) {
    assert(!running_);
    assert(threads_.empty());
    
    AtomicSetValue(running_, 1);
    AtomicSetValue(worker_, workers);

    threads_.reserve(workers);
    for (int i = 0; i < workers; i++) {
        queues_.push_back(new JobQueue);
        threads_.push_back(
            new Thread(boost::bind(&ThreadPool::WorkHandler, this, i)));
        threads_[i].Start();
    }
}

void ThreadPool::Stop() {
    AtomicSetValue(running_, 0);
    AtomicSetValue(worker_, 0);
    for (size_t i = 0; i < threads_.size(); i++) {
        threads_[i].Join();
    }

}

void ThreadPool::WorkHandler(int which) {
    QueuePtr work_queue = queues_[which];

    while (AtomicGetValue(running_)) {
        std::vector<Job*> take;
        work_queue.FetchAll(take);

        assert(!take.empty());
        for (size_t i = 0; i < take.size(); i++) {
            Job* job = take[i];
            job->functor();

        }
    }
}
