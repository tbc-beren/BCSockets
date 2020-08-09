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

class BCSocketUnix : public BCSocketBaseImpl {
public:
    BCSocketUnix()
    {}
    BCSocketUnix(int type)
    : BCSocketBaseImpl(AF_UNIX, type, 0)
    {}

    void reset(int type) {
        BCSocketBaseImpl::reset(AF_UNIX, type, 0);
    }
    void connect(const std::string& socketName) {
        struct sockaddr_un addr;
        addrFromName(addr, socketName);
        BCSocketBaseImpl::connect((sockaddr*)&addr, sizeof(struct sockaddr_un));
    }

protected:
    void addrFromName(struct sockaddr_un& addr, const std::string& socketName) {
        if (socketName.length() >= sizeof(addr.sun_path)-1) {
            throw BlackCodex::BCSockets::BCSocketException("socket name too long");
        }
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = getFamily();
        strcpy(addr.sun_path, socketName.c_str());
    }
};

} // BCSockets
} // BCTools
