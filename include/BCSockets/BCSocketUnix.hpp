/**
*
* The Black Codex Library: Chapter: BCSockets
*
* https://github.com/tbc-beren/BCSockets
*
* Mozilla Public License Version 2.0
* https://github.com/tbc-beren/BCSockets/blob/master/LICENSE
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* https://mozilla.org/MPL/2.0/.
*
*/
#pragma once

#include <BCSockets/BCSocket.hpp>
#include <BCSockets/native/BCSocketImplNative.hpp>

#include <sys/un.h>

#include <string>

namespace BlackCodex {
namespace BCSockets {

class BCSocketUnix : public BCSocketClient<BCSocketImplNative> {
public:
    BCSocketUnix()
    {}
    BCSocketUnix(int type)
    {
        reset(type);
    }
    virtual ~BCSocketUnix() {
        BCSocketClient<BCSocketImplNative>::reset();
    }

    void reset(int type) {
        BCSocketClient<BCSocketImplNative>::reset(AF_UNIX, type, 0);
    }
    void connect(const std::string& socketName) {
        struct sockaddr_un addr;
        addrFromName(addr, socketName);
        BCSocketClient<BCSocketImplNative>::connect((sockaddr*)&addr, sizeof(struct sockaddr_un));
    }

    static void addrFromName(struct sockaddr_un& addr, const std::string& socketName) {
        if (socketName.length() >= sizeof(addr.sun_path)) {
            throw BlackCodex::BCSockets::BCSocketException("socket name too long");
        }
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, socketName.c_str());
    }
};

class BCSocketUnixSrv : public BCSocketSrv<BCSocketImplNative> {
    std::string mName;
public:
    BCSocketUnixSrv()
    {}
    BCSocketUnixSrv(int type, const std::string& socketName)
    {
        reset(type, socketName);
    }
    virtual ~BCSocketUnixSrv() {
        reset();
    }
    void reset() {
        reset(AF_UNSPEC, "");
    }
    void reset(int type, const std::string& socketName) {
        mName = socketName;
        if (AF_UNSPEC == type) {
            BCSocketBase::reset();
        } else {
            BCSocketSrv<BCSocketImplNative>::reset(AF_UNIX, type, 0);
        }
    }
    void bind() override {
        struct sockaddr_un addr;
        BCSocketUnix::addrFromName(addr, mName);
        BCSocketSrv<BCSocketImplNative>::implBind((struct sockaddr*)&addr, sizeof(addr));
    }
};

} // BCSockets
} // BCTools
