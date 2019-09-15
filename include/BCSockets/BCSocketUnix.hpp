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

class BCSocketUnix : public BCSocket {
public:
    BCSocketUnix(int type)
    : BCSocket(AF_UNIX, type, 0)
    {}

    void bind(const std::string& socketName) {
        struct sockaddr_un addr;
        addrFromName(addr, socketName);
        BCSocket::bind((struct sockaddr*)&addr, sizeof(addr));
    }
    void connect(const std::string& socketName) {
        struct sockaddr_un addr;
        addrFromName(addr, socketName);
        BCSocket::connect((sockaddr*)&addr, sizeof(struct sockaddr_un));
    }

protected:
    void addrFromName(struct sockaddr_un& addr, const std::string& socketName) {
        if (socketName.length() >= sizeof(addr.sun_path)-1) {
            throw BlackCodex::BCSockets::BCSocketException("socket name too long");
        }
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, socketName.c_str());
    }
};

} // BCSockets
} // BCTools
