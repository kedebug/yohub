#include "network/event_pool.h"
#include "network/async_server.h"
#include "network/inet_address.h"
#include <signal.h>
#include <boost/bind.hpp>

using namespace yohub;

class EchoServer {
  public:
    EchoServer(EventPool* event_pool, const InetAddress& bindaddr)
        : event_pool_(event_pool),
          async_server_(event_pool_, bindaddr)
    {
        async_server_.SetConnectionCallback(
            boost::bind(&EchoServer::OnConnection, this, _1));
        async_server_.SetWriteCompletionCallback(
            boost::bind(&EchoServer::OnReadCompletion, this, _1));
    }

    void Start() {
    }

  private:
    void OnConnection(AsyncConnection* conn) {

    }
    
    void OnReadCompletion(AsyncConnection* conn) {
    
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
