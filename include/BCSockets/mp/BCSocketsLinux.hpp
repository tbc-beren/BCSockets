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

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef int bcsocket_t;
#define INVALID_SOCKET (-1)
#define closesocket(sock) close(sock)

namespace BlackCodex {
namespace BCSockets {

class BCSocketsApp
{
public:
    BCSocketsApp()
    {}

    virtual ~BCSocketsApp()
    {}
};

}
}
