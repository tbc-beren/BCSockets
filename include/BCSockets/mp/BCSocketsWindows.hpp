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

#include <WinSock.h>
#pragma comment(lib, "Ws2_32.lib")

typedef SOCKET bcsocket_t;

namespace BlackCodex {
namespace BCSockets {

class BCSocketsApp
{
    int     mInitResult;
    WSADATA mWsaData;

public:
    BCSocketsApp()
        : mInitResult(WSAStartup(MAKEWORD(2,2), &mWsaData))
    {
        if (mInitResult != 0){
            throw std::runtime_error("Failed to start WinSock2");
        }
    }

    virtual ~BCSocketsApp() {
        WSACleanup();
    }
};

}
}
