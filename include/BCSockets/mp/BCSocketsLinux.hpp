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

#ifdef BCSOCKETS_OPENSSL

#include <openssl/ssl.h>
#include <openssl/conf.h>

#endif // BCSOCKETS_OPENSSL

typedef int bcsocket_t;
#define INVALID_SOCKET (-1)
#define closesocket(sock) close(sock)

namespace BlackCodex {
namespace BCSockets {

class BCSocketsApp
{
public:
    BCSocketsApp()
    {
#ifdef BCSOCKETS_OPENSSL
        SSL_library_init();
        SSLeay_add_ssl_algorithms();
        SSL_load_error_strings();
#endif
    }

    virtual ~BCSocketsApp()
    {
#ifdef BCSOCKETS_OPENSSL
        EVP_cleanup();
#endif
    }
};

}
}
