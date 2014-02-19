#ifndef _YOHUB_NETWORK_CALLBACKS_H_
#define _YOHUB_NETWORK_CALLBACKS_H_

#include <boost/function.hpp>

namespace yohub {

class AsyncConnection;

typedef boost::function<void (AsyncConnection*)> ConnectionCallback;
typedef boost::function<void (AsyncConnection*)> WriteCompletionCallback;
typedef boost::function<void (AsyncConnection*)> ReadCompletionCallback;
typedef boost::function<void (AsyncConnection*)> CloseCallback;

} // namespace yohub

#endif // _YOHUB_NETWORK_CALLBACKS_H_
