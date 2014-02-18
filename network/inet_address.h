#ifndef _YOHUB_NETWORK_INET_ADDRESS_H_
#define _YOHUB_NETWORK_INET_ADDRESS_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

namespace yohub {

class InetAddress {
  public:
    typedef uint16_t Port;

    InetAddress(Port port);
    InetAddress(const std::string& ip, Port port);
    InetAddress(const sockaddr_in& inet_addr);

    void SetSockAddr(const sockaddr_in& inet_addr) {
        inet_addr_ = inet_addr;
    }

    uint16_t port() const {
        return ntohs(inet_addr_.sin_port);
    }

    std::string ip() const {
        char ip[32];
        ::inet_ntop(AF_INET, &inet_addr_.sin_addr, ip, sizeof(ip));
        return ip;
    }

    const struct sockaddr_in& sockaddr_in() const {
        return inet_addr_;
    }

  private:
    struct sockaddr_in inet_addr_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_INET_ADDRESS_H_
