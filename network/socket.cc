#include "network/socket.h"
#include "network/inet_address.h"
#include "share/log.h"
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>

using namespace yohub;

Socket::Socket(int fd)
    : socket_fd_(fd)
{
}

Socket::~Socket() {
    ::close(socket_fd_);
}

void Socket::Bind(const InetAddress& addr) {
    int result = ::bind(socket_fd_, 
            reinterpret_cast<const sockaddr*>(&addr.sockaddr_in()),
            static_cast<socklen_t>(sizeof(sockaddr_in)));
    if (result < 0) {
        LOG_FATAL("bind failed, error: %s", strerror(errno));
    }
}

void Socket::Listen() {
    int result = ::listen(socket_fd_, SOMAXCONN);
    if (result < 0) {
        LOG_FATAL("listen failed, error: %s", strerror(errno));
    }
}

int Socket::Accept(InetAddress* peeraddr) {
  retry:
    struct sockaddr_in sa;
    socklen_t len = sizeof(sa);

    memset(&sa, 0, sizeof(sa));
    int newfd = ::accept(socket_fd_, reinterpret_cast<sockaddr*>(&sa), &len);

    if (newfd < 0) {
        if (errno == EAGAIN || errno == ECONNABORTED)
            goto retry;
        LOG_WARNING("accept failed, error: %s", strerror(errno));
    } else {
        peeraddr->SetSockAddr(sa);
    }
    return newfd;
}

void Socket::SetTcpNoDelay(bool opt) {
    int value = opt ? 1 : 0;
    ::setsockopt(socket_fd_, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
}

void Socket::SetReuseAddr(bool opt) {
    int value = opt ? 1 : 0;
    ::setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));
}

void Socket::SetReusePort(bool opt) {
#ifdef SO_REUSEPORT
    int value = opt ? 1 : 0;
    ::setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEPORT, &value, sizeof(value));
#endif
}

void Socket::SetKeepAlive(bool opt) {
    int value = opt ? 1 : 0;
    ::setsockopt(socket_fd_, SOL_SOCKET, SO_KEEPALIVE, &value, sizeof(value));
}

int Socket::CreateNonblockingSocket() {
    int newfd = ::socket(AF_INET, SOCK_NONBLOCK | SOCK_CLOEXEC | SOCK_STREAM, IPPROTO_TCP);
    if (newfd < 0) {
        LOG_FATAL("socket failed, error: %s", strerror(errno));
    }
    return newfd;
}
