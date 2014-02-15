#ifndef _YOHUB_SHARE_ASYNC_H_
#define _YOHUB_SHARE_ASYNC_H_

#include "share/atomic.h"

namespace yohub {

class AsyncContext;

class AsyncClient {
  public:
    AsyncClient() { }
    virtual ~AsyncClient() { }
    virtual void OnCompletion(AsyncContext* ctx) = 0;
};

class AsyncContext {
  public:
    AsyncContext(AsyncClient* client);
    virtual ~AsyncContext() { }

    void SetReadEvent();
    void SetWriteEvent();

    int Ref(); 
    int Unref();

    AsyncClient* client() const {
        return client_;
    }

  private:
    volatile int refs_;
    static volatile uint32_t s_num_instance_;
    uint32_t sequence_number_;
    AsyncClient* client_;
};

} // namespace yohub

#endif // _YOHUB_SHARE_ASYNC_H_
