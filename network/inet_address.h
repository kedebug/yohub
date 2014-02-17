#ifndef _YOHUB_NETWORK_INET_ADDRESS_H_
#define _YOHUB_NETWORK_INET_ADDRESS_H_

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

    const struct sockaddr_in& sockaddr_in() const {
        return inet_addr_;
    }

  private:
    struct sockaddr_in inet_addr_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_INET_ADDRESS_H_
