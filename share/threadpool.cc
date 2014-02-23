#include "share/threadpool.h"
#include "share/log.h"
#include <vector>
#include <boost/bind.hpp>

using namespace yohub;

ThreadPool::ThreadPool()
    : running_(0),
      workers_(0),
      job_count_(0)
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
    AtomicSetValue(workers_, workers);

    threads_.reserve(workers);
    for (int i = 0; i < workers; i++) {
        queues_.push_back(new JobQueue);
        threads_.push_back(
            new Thread(boost::bind(&ThreadPool::WorkHandler, this, i)));
        threads_[i].Start();
    }
}

void ThreadPool::Stop() {
    if (AtomicSetValue(running_, 0) == 1) {
        for (size_t i = 0; i < threads_.size(); i++)
            threads_[i].Join();
        assert(AtomicGetValue(job_count_) == 0);
    }
}

void ThreadPool::WorkHandler(int which) {
    JobQueue::Container jobs;
    while (AtomicGetValue(running_) == 1 ||
           AtomicGetValue(job_count_) > 0) {
        jobs.clear();
        queues_[which].FetchAll(&jobs, 1);

        for (size_t i = 0; i < jobs.size(); i++) {
            jobs[i]();
            AtomicDec(job_count_);
        }
    }
}

void ThreadPool::Schedule(Job job, int which) {
    int jobs = AtomicInc(job_count_);
    if (which == -1) {
        which = jobs % workers_;
    }
    queues_[which].Push(job);
}
