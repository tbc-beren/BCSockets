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

#include "mockEngine/BCSocketMockEngine.h"

#include <BCSockets/BCSocketsApp.hpp>
#include <BCSockets/BCServer.hpp>

namespace BlackCodex {
namespace BCSockets {

typedef BCSocketMockEngine BCSocketImplMock;

class BCSocketMock : public BlackCodex::BCSockets::BCSocketClient<BCSocketImplMock> {
public:
    BCSocketMock()
    {}
    BCSocketMock(int type)
    {
        BCSocketMock::reset(type);
    }
    virtual ~BCSocketMock() {
        BCSocketClient<BCSocketImplMock>::reset();
    }
    void reset(int type) {
        BCSocketClient<BCSocketImplMock>::reset(AF_UNIX, type, 0);
    }
    void connect(const std::string& socketName) {
        struct sockaddr addr;
        addrFromName(addr, socketName);
        BCSocketClient<BCSocketImplMock>::connect(&addr, socketName.size());
    }

    static
    void addrFromName(struct sockaddr& addr, const std::string& socketName) {
        addr.sa_family = 0XB1AC;
        const char** data = (const char**)addr.sa_data;
        *data = socketName.c_str();
    }
};

class BCSocketSrvMock : public BlackCodex::BCSockets::BCSocketSrv<BCSocketImplMock> {
    std::string mName;
public:
    BCSocketSrvMock()
    {}
    BCSocketSrvMock(int type, const std::string& socketName)
    {
        reset(type, socketName);
    }
    virtual ~BCSocketSrvMock() {
        reset();
    }
    void reset() {
        mName.clear();
        BCSocketBase::reset();
    }
    void reset(int type, const std::string& socketName) {
        mName = socketName;
        BCSocketSrv<BCSocketImplMock>::reset(AF_UNSPEC, type, 0);
    }
    void bind() override {
        struct sockaddr addr;
        BCSocketMock::addrFromName(addr, mName);
        BCSocketSrv<BCSocketImplMock>::implBind(&addr, mName.size());
    }
};

} // BCSockets
} // BCTools
