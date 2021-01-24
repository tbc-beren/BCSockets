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
#include <thread>

namespace BlackCodex {
namespace BCSockets {

BCSocketMockException::BCSocketMockException(const char* message) 
: BCSocketException(message)
{}
BCSocketMockException::~BCSocketMockException()
{}

BCSocketMockInternal::BCSocketMockInternal()
: mListenBacklog(-1)
, mRemotePeer(INVALID_SOCKET)
{}
BCSocketMockInternal::~BCSocketMockInternal()
{}
int BCSocketMockInternal::connect(const std::string& peerName) {
    if (!mPeerBind.empty()) {
        // Already Binded!
        return -1;
    }
    mPeer = peerName;
    int rv = BCSocketMockEngine::findServerPeer(*this);
    return rv;
}
int BCSocketMockInternal::bind(const std::string& peerName) {
    if (!mPeerBind.empty()) {
        return -1;
    }
    mPeerBind = peerName;
    return 0;
}
int BCSocketMockInternal::listen(int backlog) {
    if (mListenBacklog == backlog) {
        return -1;
    }
    mListenBacklog = backlog;
    return 0;
}
bcsocket_t BCSocketMockInternal::accept(struct sockaddr* addr, socklen_t* addrlen) {
    (void)(addr);
    (void)(addrlen);
    if (mBacklog.size() > 0) {
        bcsocket_t sockRemote = mBacklog.back();
        mBacklog.pop();

        auto& objRemote = BCSocketMockEngine::socketGet(sockRemote);
        auto& sockNewObj = BCSocketMockEngine::socketNew(objRemote.getFamily(), objRemote.getType(), objRemote.getProto());
        sockNewObj.mRemotePeer = objRemote.get();
        objRemote.mRemotePeer = sockNewObj.get();

        return sockNewObj.get();
    }
    return INVALID_SOCKET;
}

int BCSocketMockInternal::receiveConnection(BCSocketMockInternal& remoteSocket) {
    if (INVALID_SOCKET == remoteSocket.mSocket ||
        INVALID_SOCKET != mRemotePeer) {
        return -1;
    }
    if (remoteSocket.mPeer == mPeerBind) {
        mBacklog.emplace(remoteSocket.mSocket);

        // Wait for accept...
        int tries = 0;
        do {
            std::this_thread::sleep_for( std::chrono::milliseconds(50) );
        } while (tries ++ < 10 && INVALID_SOCKET == remoteSocket.mRemotePeer);

        return INVALID_SOCKET == remoteSocket.mRemotePeer? -1 : 0;
    }
    return -1;
}

int BCSocketMockInternal::implErrno() const {
    return BCSocketMockEngine::implErrno();
}

int BCSocketMockInternal::internalWrite(const void* buffer, int bufferlen, int flags) {
    (void)flags;
    auto& peer = BCSocketMockEngine::socketGet(mRemotePeer);
    peer.mData.append( std::string((const char*)buffer, bufferlen) );
    return bufferlen;
}

int BCSocketMockInternal::internalRead(const void* buffer, int bufferlen, int flags) {
    (void)flags;
    for (int i = 0; i < 10; i++) {
        if (mData.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        int readed = std::min(bufferlen, (int)mData.size());
        memcpy((void*)buffer, mData.c_str(), readed);
        mData.erase(0,readed);
        return readed;
    }
    return 0;
}

} // namespace BCSockets
} // namespace BlackCodex
