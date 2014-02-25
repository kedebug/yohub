#ifndef _YOHUB_SHARE_UTILS_H_
#define _YOHUB_SHARE_UTILS_H_

#include <string.h>
#include "share/log.h"

namespace yohub {
namespace utils {

inline void PthreadCall(const char* label, int result) {
    if (result != 0) {
        LOG_FATAL("pthread %s: %s\n", label, strerror(result));
    }
}

} // namespace utils
} // namespace yohub

#endif // _YOHUB_SHARE_UTILS_H_
