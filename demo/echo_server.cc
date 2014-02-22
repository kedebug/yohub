#include "network/event_pool.h"
#include "network/async_server.h"
#include "network/inet_address.h"
#include "network/buffer.h"
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
    void OnConnection(AsyncConnection* conn) {
        LOG_TRACE("local=%s:%d, peer=%s:%d",
            conn->local_addr().ip().c_str(), conn->local_addr().port(),
            conn->peer_addr().ip().c_str(), conn->peer_addr().port()); 
    }
    
    void OnReadCompletion(AsyncConnection* conn, Buffer* buffer) {
        std::string s(buffer->TakeAsString());
        LOG_TRACE("receive message: %s", s.c_str());
    }

    EventPool* event_pool_;
    AsyncServer async_server_;
};

static bool stop = false;

void SignalStop(int) {
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
