#include "network/inet_address.h"
#include "share/log.h"

using namespace yohub;

InetAddress::InetAddress(const struct sockaddr_in& inet_addr)
    : inet_addr_(inet_addr)
{
}

InetAddress::InetAddress(Port port) {
    memset(&inet_addr_, 0, sizeof(inet_addr_));
    inet_addr_.sin_family = AF_INET;
    inet_addr_.sin_addr.s_addr = INADDR_ANY;
    inet_addr_.sin_port = htons(port);
}

InetAddress::InetAddress(const std::string& ip, Port port) {
    memset(&inet_addr_, 0, sizeof(inet_addr_));
    inet_addr_.sin_family = AF_INET;
    inet_addr_.sin_port = htons(port);
    ::inet_pton(AF_INET, ip.c_str(), &inet_addr_.sin_addr);
}

