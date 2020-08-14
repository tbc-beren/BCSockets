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

#include <sys/un.h>

#include <string>

namespace BlackCodex {
namespace BCSockets {

class BCSocketUnix : public BCSocketClient<BCSocketImplEx> {
public:
    BCSocketUnix()
    {}
    BCSocketUnix(int type)
    {
        reset(type);
    }

    void reset(int type) {
        BCSocketClient<BCSocketImplEx>::reset(AF_UNIX, type, 0);
    }
    void connect(const std::string& socketName) {
        struct sockaddr_un addr;
        addrFromName(addr, socketName);
        BCSocketClient<BCSocketImplEx>::connect((sockaddr*)&addr, sizeof(struct sockaddr_un));
    }

    static void addrFromName(struct sockaddr_un& addr, const std::string& socketName) {
        if (socketName.length() >= sizeof(addr.sun_path)-1) {
            throw BlackCodex::BCSockets::BCSocketException("socket name too long");
        }
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, socketName.c_str());
    }
};

class BCSocketUnixSrv : public BCSocketSrv<BCSocketImplEx> {
    std::string mName;
public:
    BCSocketUnixSrv()
    {}
    BCSocketUnixSrv(int type, const std::string& socketName)
    {
        reset(type, socketName);
    }
    void reset() {
        reset(0, "");
    }
    void reset(int type, const std::string& socketName) {
        mName = socketName;
        BCSocketSrv<BCSocketImplEx>::reset(AF_UNIX, type, 0);
    }
    void bind() override {
        struct sockaddr_un addr;
        BCSocketUnix::addrFromName(addr, mName);
        BCSocketSrv<BCSocketImplEx>::implBind((struct sockaddr*)&addr, sizeof(addr));
    }
};

} // BCSockets
} // BCTools
