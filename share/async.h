#ifndef _YOHUB_SHARE_ASYNC_H_
#define _YOHUB_SHARE_ASYNC_H_

#include <boost/function.hpp>

namespace yohub {

struct Job;
typedef boost::function<void (Job*)> JobProc;

struct Job {
    JobProc functor;
};

} // namespace yohub

#endif // _YOHUB_SHARE_ASYNC_H_
