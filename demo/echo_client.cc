#include "share/log.h"
#include "network/event_pool.h"
#include "network/async_client.h"
#include "network/async_connection.h"
#include "network/buffer.h"
#include <string>
#include <boost/bind.hpp>
#include <signal.h>

using namespace yohub;

static bool stop = false;

class EchoClient {
  public:
    EchoClient(EventPool* event_pool, const InetAddress& remote)
        : event_pool_(event_pool),
          async_client_(event_pool_, remote) 
    { 
        async_client_.SetConnectionCallback(
            boost::bind(&EchoClient::OnConnection, this, _1));
        async_client_.SetReadCompletionCallback(
            boost::bind(&EchoClient::OnReadCompletion, this, _1, _2));
    }

    ~EchoClient() {
        async_client_.Disconnect();
    }

    void Connect() {
        async_client_.Connect();
    }

  private:
    void OnConnection(const AsyncConnectionPtr& conn) {
        LOG_TRACE("local=%s:%d, peer=%s:%d, %s",
            conn->local_addr().ip().c_str(), conn->local_addr().port(),
            conn->peer_addr().ip().c_str(), conn->peer_addr().port(),
            conn->connected() ? "connected" : "disconnected"); 

        conn->Write(std::string(" world "));
    }

    void OnReadCompletion(const AsyncConnectionPtr& conn, Buffer* buffer) {
        std::string s(buffer->TakeAsString());
        LOG_TRACE("received: %s", s.c_str());
        conn->Write(s.data(), s.size());
    }

    EventPool* const event_pool_;
    AsyncClient async_client_;
};

void SignalStop(int) {
    LOG_TRACE("Stop running...");
    stop = true;
}

int main() {
    ::signal(SIGINT, SignalStop);    

    log::SetLogLevel(log::TRACE);

    EventPool event_pool(1, 1);
    event_pool.Run();

    InetAddress remote("127.0.0.1", 19910);
    EchoClient client(&event_pool, remote);
    client.Connect();

    while (true) {
        if (stop) {
            event_pool.Stop();
            break;
        }
        ::usleep(1000);
    }
}
