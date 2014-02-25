#ifndef _YOHUB_SHARE_LOG_H_
#define _YOHUB_SHARE_LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace yohub {

#define LOG_NOTICE(fmt,...) fprintf(stderr,"[NOTICE][%s:%d]" fmt "\n",__FILE__,__LINE__,##__VA_ARGS__)
#define LOG_TRACE(fmt,...) fprintf(stderr,"[TRACE][%s:%d]" fmt "\n",__FILE__,__LINE__,##__VA_ARGS__)
#ifdef DEBUG
#define LOG_DEBUG(fmt,...) fprintf(stderr,"[DEBUG][%s:%d]" fmt "\n",__FILE__,__LINE__,##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt,...)
#endif
#define LOG_WARN(fmt,...) fprintf(stderr,"[WARNING][%s:%d]" fmt "\n",__FILE__,__LINE__,##__VA_ARGS__)
#define LOG_FATAL(fmt,...) fprintf(stderr,"[FATAL][%s:%d]" fmt "\n",__FILE__,__LINE__,##__VA_ARGS__);abort();

} // namespace yohub

#endif // _YOHUB_SHARE_LOG_H_
