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
#include "BCSocketMockEngine.h"

#include <cstring>

namespace BlackCodex {
namespace BCSockets {

//static
BCSocketMockEngine& BCSocketMockEngine::engine() {
    static BCSocketMockEngine gEngine;
    return gEngine;
}
//static
bcsocket_t BCSocketMockEngine::newId() {
    auto& me = engine();
    return me.mIdSocketMax++;
}
//static
std::string BCSocketMockEngine::peerNameFromAddr(const sockaddr& addr, int addrlen) {
    if (addr.sa_family != 0xB1AC) {
        throw BCSocketMockException("invalid socket family");
    }
    const char* peerNameRaw = "";
    memcpy(&peerNameRaw, addr.sa_data, sizeof(void*));
    return std::string(peerNameRaw, addrlen);
}
BCSocketMockEngine::BCSocketMockEngine()
: mErrno(0)
, mIdSocketMax(1)
{}
//static
int BCSocketMockEngine::setError(int errCode) {
    engine().mErrno = errCode;
    return -1;
}
bool BCSocketMockEngine::findServerPeerByName(const std::string& peerName) {
    for (const auto& sock : mSockets) {
        if (peerName == sock.second.mPeerBind) {
            return true;
        }
    }
    return false;
}
BCSocketMockEngine::~BCSocketMockEngine()
{}
//static
BCSocketMockInternal& BCSocketMockEngine::socketNew(int af, int type, int proto) {
    auto& me = engine();
    bcsocket_t idNew = BlackCodex::BCSockets::BCSocketMockEngine::newId();
    auto& objSock = me.mSockets[idNew];
    objSock.reset(af, type, proto, idNew);
    return objSock;
}
//static
BCSocketMockInternal& BCSocketMockEngine::socketGet(bcsocket_t sock) {
    auto& me = engine();
    auto it = me.mSockets.find(sock);
    if (me.mSockets.end() == it) {
        throw BCSocketMockException("socket not found");
    }
    return it->second;
}
//static
bcsocket_t BCSocketMockEngine::implSocket(int af, int type, int proto) {
    auto idNew = engine().socketNew(af, type, proto).get();
    return idNew;
}
//static
void BCSocketMockEngine::implClose(bcsocket_t sock) {
    auto& me = engine();
    auto it = me.mSockets.find(sock);
    if (me.mSockets.end() == it) {
        throw BCSocketMockException("socket not found");
    }
    it->second.reset();
    me.mSockets.erase(it);
}
//static
int BCSocketMockEngine::implRecv(bcsocket_t sock, void* buffer, int bufferlen, int flags) {
    auto& sockObj = engine().socketGet(sock);
    return sockObj.internalRead(buffer, bufferlen, flags);
}
//static
int BCSocketMockEngine::implSend(bcsocket_t sock, const void* buffer, int bufferlen, int flags) {
    auto& sockObj = engine().socketGet(sock);
    return sockObj.internalWrite(buffer, bufferlen, flags);
}

//static
int BCSocketMockEngine::implConnect(bcsocket_t sock, const sockaddr* addr, int addrlen) {
    const std::string peerName = peerNameFromAddr(*addr, addrlen);
    return engine().socketGet(sock).connect(peerName);
}
//static
int BCSocketMockEngine::implBind(bcsocket_t sock, const sockaddr* addr, int addrlen) {
    std::string peerName = peerNameFromAddr(*addr, addrlen);
    auto& me = engine();
    if (me.findServerPeerByName(peerName)) {
        return setError(EADDRINUSE);
    }
    return me.socketGet(sock).bind(peerName);
}
//static
int BCSocketMockEngine::implListen(bcsocket_t sock, int backlog) {
    auto& me = engine();
    auto& x = me.socketGet(sock);
    return x.listen(backlog);
}
//static
bcsocket_t BCSocketMockEngine::implAccept(int sock, struct sockaddr *addr, socklen_t *addrlen) {
    return engine().socketGet(sock).accept(addr, addrlen);
}
//static
int BCSocketMockEngine::implSelect(bcsocket_t nfds, fd_set *readfds, fd_set *writefds,
                                   fd_set *exceptfds, struct timeval *timeout) {
    if (nullptr != writefds || nullptr != exceptfds) {
        throw BCSocketMockException("Unsupported select()");
    }

    struct timespec tim, tim2;

    auto& me = engine();
    do {
        for(auto& item : me.mSockets) {
            if (item.first < nfds && (
                    item.second.mBacklog.size() ||
                    !item.second.mData.empty()
                    )
                ) {
                FD_CLR(item.first, readfds);
                return 1;
            }
        }

        tim.tv_sec = timeout->tv_sec;
        tim.tv_nsec = timeout->tv_usec * 1000;
        nanosleep(&tim , &tim2);

    } while(0 == tim2.tv_nsec && 0 == tim2.tv_sec);

    return 0;
}
//static
int BCSocketMockEngine::implErrno() {
    return engine().mErrno;
}
//static
int BCSocketMockEngine::findServerPeer(BCSocketMockInternal& peerConnect) {
    auto& me = engine();
    for (auto& item : me.mSockets) {
        int rv = item.second.receiveConnection(peerConnect);
        if (rv >= 0) {
            return rv;
        }
    }
    return -1;
}

} // BCSockets
} // BlackCodex
