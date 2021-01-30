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

#include <BCSockets/BCSocketsApp.hpp>

namespace BlackCodex {
namespace BCSockets {

class BCSocketImplNative
{
public:
    static bcsocket_t implSocket(int af, int type, int proto) {
        return ::socket(af, type, proto);
    }
    static int implConnect(bcsocket_t sock, const sockaddr* addr, int addrlen) {
        return ::connect(sock, addr, addrlen);
    }
    static void implClose(bcsocket_t sock) {
        ::closesocket(sock);
    }
    static int implRecv(bcsocket_t sock, void* buffer, int bufferlen, int flags) {
        return ::recv(sock, buffer, bufferlen, flags);
    }
    static int implSend(bcsocket_t sock, const void* buffer, int bufferlen, int flags) {
        return ::send(sock, buffer, bufferlen, flags);
    }
    static int implSelect(bcsocket_t nfds, fd_set *readfds, fd_set *writefds,
                          fd_set *exceptfds, struct timeval *timeout) {
        return ::select(nfds, readfds, writefds, exceptfds, timeout);
    }
    static int implListen(bcsocket_t sock, int backlog) {
        return ::listen(sock, backlog);
    }
    static int  implBind(bcsocket_t sock, const sockaddr* addr, int addrlen) {
        return ::bind(sock, addr, addrlen);
    }
    static int implAccept(int sock, struct sockaddr *addr, socklen_t *addrlen) {
        return ::accept(sock, addr, addrlen);
    }
    static int implErrno(){
        return errno;
    }
};

} // BCSockets
} // BCTools
