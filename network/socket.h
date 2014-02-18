#ifndef _YOHUB_NETWORK_SOCKET_H_
#define _YOHUB_NETWORK_SOCKET_H_

#include <netinet/in.h>
#include <boost/noncopyable.hpp>

namespace yohub {

class InetAddress;

class Socket : boost::noncopyable {
  public:
    Socket(int fd);
    ~Socket();

    void Bind(const InetAddress& address);
    void Listen();
    int Accept(InetAddress* peer_address);

    void SetTcpNoDelay(bool opt);
    void SetReuseAddr(bool opt);
    void SetReusePort(bool opt);
    void SetKeepAlive(bool opt);

    int fd() const { return socket_fd_; }

    static int CreateNonblockingSocket();
    static struct sockaddr_in GetSocketName(int sockfd);

  private:
    const int socket_fd_;
};

} // namespace yohub

#endif // _YOHUB_NETWORK_SOCKET_H_
