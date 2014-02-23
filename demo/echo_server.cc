#include "network/event_pool.h"
#include "network/async_server.h"
#include "network/inet_address.h"
#include "network/buffer.h"
#include "network/callbacks.h"
#include "share/log.h"
#include <boost/bind.hpp>
#include <string>
#include <signal.h>

using namespace yohub;

class EchoServer {
  public:
    EchoServer(EventPool* event_pool, const InetAddress& bindaddr)
        : event_pool_(event_pool),
          async_server_(event_pool_, bindaddr)
    {
        async_server_.SetConnectionCallback(
            boost::bind(&EchoServer::OnConnection, this, _1));
        async_server_.SetReadCompletionCallback(
            boost::bind(&EchoServer::OnReadCompletion, this, _1, _2));
    }

    void Start() {
        async_server_.Start();
    }

  private:
    void OnConnection(const AsyncConnectionPtr& conn) {
        LOG_TRACE("local=%s:%d, peer=%s:%d, %s",
            conn->local_addr().ip().c_str(), conn->local_addr().port(),
            conn->peer_addr().ip().c_str(), conn->peer_addr().port(),
            conn->connected() ? "connected" : "disconnected"); 
    }
    
    void OnReadCompletion(const AsyncConnectionPtr& conn, Buffer* buffer) {
        std::string s(buffer->TakeAsString());
        LOG_TRACE("receive message: %zu", s.size());
    }

    EventPool* event_pool_;
    AsyncServer async_server_;
};

static bool stop = false;

void SignalStop(int) {
    LOG_TRACE("Stop running...");
    stop = true;
}

int main() {
    ::signal(SIGINT, SignalStop);

    EventPool event_pool(1, 1);
    InetAddress bindaddr("127.0.0.1", 19910);
    event_pool.Run();
    EchoServer server(&event_pool, bindaddr);
    server.Start();

    while (true) {
        if (stop) {
            event_pool.Stop();
            break;
        }
        ::sleep(1);
    }
}
