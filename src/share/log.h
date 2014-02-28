#ifndef _YOHUB_SHARE_LOG_H_
#define _YOHUB_SHARE_LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace yohub {

namespace log {

enum LogLevel {
    NOTICE, TRACE, DEBUG, WARN, FATAL
};

extern LogLevel g_loglevel;

inline LogLevel GetLogLevel() { return g_loglevel; }
inline void SetLogLevel(LogLevel level) { g_loglevel = level; }

} // namespace log


#define LOG_NOTICE(fmt, ...) if (yohub::log::GetLogLevel() <= yohub::log::NOTICE) \
    fprintf(stderr, "[NOTICE][%s:%d]" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_TRACE(fmt, ...) if (yohub::log::GetLogLevel() <= yohub::log::TRACE) \
    fprintf(stderr, "[TRACE][%s:%d]" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) if (yohub::log::GetLogLevel() <= yohub::log::DEBUG) \
    fprintf(stderr, "[DEBUG][%s:%d]" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) if (yohub::log::GetLogLevel() <= yohub::log::WARN) \
    fprintf(stderr, "[WARN][%s:%d]" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) if (yohub::log::GetLogLevel() <= yohub::log::FATAL) \
    fprintf(stderr, "[FATAL][%s:%d]" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);abort();

} // namespace yohub

#endif // _YOHUB_SHARE_LOG_H_
