#ifndef _YOHUB_NETWORK_CALLBACKS_H_
#define _YOHUB_NETWORK_CALLBACKS_H_

#include <boost/function.hpp>

namespace yohub {

class AsyncConnection;

typedef boost::function<void (AsyncConnection*)> ConnectionCallback;
typedef boost::function<void (AsyncConnection*)> WriteCompleteCallback;
typedef boost::function<void (AsyncConnection*)> ReadCompleteCallback;

} // namespace yohub

#endif // _YOHUB_NETWORK_CALLBACKS_H_
