/**
*
* The Black Codex Library: Chapter: BCSockets
*
* https://github.com/tbc-beren/BCSockets
*
* Mozilla Public License Version 2.0
* https://github.com/tbc-beren/BCSockets/blob/master/LICENSE
*
*/
#pragma once

#include <BCSockets/BCSocket.hpp>

#include <cstring>

namespace BlackCodex {
namespace BCSockets {

class BCSocketNet : public BCSocketClient<BCSocketImplEx> {
    struct sockaddr_in mAddr;
public:
    BCSocketNet()
    {}
    BCSocketNet(int af, int type, int protocol)
    : BCSocketClient<BCSocketImplEx>()
    {
        reset(af, type, protocol);
    }
};

class BCSocketNetSrv : public BCSocketSrv<BCSocketImplEx> {
    int mPort;

public:
    BCSocketNetSrv()
    : mPort(-1)
    {}

    BCSocketNetSrv(int af, int type, int protocol, int port)
    : mPort(-1)
    {
        resetPort(af, type, protocol, port);
    }

    void bind() override {
        int one = 1;
        setsockopt(get(), SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = getFamily();
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(mPort);

        const sockaddr* addr = (const sockaddr*)&serv_addr;
        implBind(addr, sizeof(serv_addr));
    }

    void reset() {
        BCSocketBase::reset(INVALID_SOCKET);
    }
    bcsocket_t getHandler() const {
        return get();
    }

protected:
    void resetPort(int af, int type, int protocol, int port) {
        mPort = port;
        BCSocketSrv<BCSocketImplEx>::reset(af, type, protocol);
    }
};

} // BCSockets
} // BCTools
