#include "network/inet_address.h"
#include <assert.h>

using namespace yohub;

int main() {
    InetAddress addr0(1234);
    assert(addr0.ip() == std::string("0.0.0.0"));
    assert(addr0.port() == 1234);

    InetAddress addr1("1.2.3.4", 8888);
    assert(addr1.ip() == std::string("1.2.3.4"));
    assert(addr1.port() == 8888);

    InetAddress addr2("255.254.253.252", 65535);
    assert(addr2.ip() == std::string("255.254.253.252"));
    assert(addr2.port() == 65535);
}
