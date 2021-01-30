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
