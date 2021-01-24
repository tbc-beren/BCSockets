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

#include <BCSockets/BCSocketsApp.hpp>
#include <BCSockets/BCSocketBase.hpp>

#include <map>
#include <queue>

namespace BlackCodex {
namespace BCSockets {

class BCSocketMockException : public BCSocketException {
public:
    BCSocketMockException(const char* message);
    virtual ~BCSocketMockException();
};

class BCSocketMockInternal : public BlackCodex::BCSockets::BCSocketBase
{
public:
    int                     mListenBacklog;
    std::string             mPeer;

    std::string             mPeerBind;
    std::queue<bcsocket_t>  mBacklog;

    bcsocket_t              mRemotePeer;

    std::string             mData;

public:
    BCSocketMockInternal();
    virtual ~BCSocketMockInternal();

    int         connect(const std::string& peerName);
    int         bind(const std::string& peerName);
    int         listen(int backlog);
    bcsocket_t  accept(struct sockaddr* addr, socklen_t* addrlen);

    int         receiveConnection(BCSocketMockInternal& remoteSocket);

    int         implErrno() const override;

    int         internalWrite(const void* buffer, int bufferlen, int flags);
    int         internalRead (const void* buffer, int bufferlen, int flags);
};

class BCSocketMockEngine
{
    int         mErrno;
    bcsocket_t  mIdSocketMax;
    std::map<bcsocket_t, BCSocketMockInternal>  mSockets;

    static BCSocketMockEngine&  engine();
    static bcsocket_t           newId();
    static std::string          peerNameFromAddr(const sockaddr& addr, int addrlen);

    BCSocketMockEngine();

    bool    findServerPeerByName(const std::string& peerName);

public:
    virtual ~BCSocketMockEngine();

    static int                      setError(int errCode);

    static BCSocketMockInternal&    socketNew(int af, int type, int proto);
    static BCSocketMockInternal&    socketGet(bcsocket_t sock);
    static bcsocket_t               implSocket(int af, int type, int proto);
    static void                     implClose(bcsocket_t sock);
    static int                      implRecv(bcsocket_t sock, void* buffer, int bufferlen, int flags);
    static int                      implSend(bcsocket_t sock, const void* buffer, int bufferlen, int flags);
    static int                      implConnect(bcsocket_t sock, const sockaddr* addr, int addrlen);
    static int                      implBind(bcsocket_t sock, const sockaddr* addr, int addrlen);
    static int                      implListen(bcsocket_t sock, int backlog);
    static bcsocket_t               implAccept(int sock, struct sockaddr *addr, socklen_t *addrlen);
    static int                      implSelect(bcsocket_t nfds, fd_set *readfds, fd_set *writefds,
                                               fd_set *exceptfds, struct timeval *timeout);
    static int                      implErrno();
    static int                      findServerPeer(BCSocketMockInternal& peerConnect);
};

} // BCSockets
} // BCTools
