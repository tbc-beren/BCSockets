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

#include <BCSockets/BCSocketException.hpp>
#include <BCSockets/BCSocketBase.hpp>

namespace BlackCodex {
namespace BCSockets {

class BCSocketImplEx
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
};

template<typename TImpl>
class BCSocketClient : public BCSocketBase
{
public:
    BCSocketClient()
    {}
    BCSocketClient(int af, int type, int proto, bcsocket_t s)
    : BCSocketBase(af, type, proto, s)
    {}
    virtual ~BCSocketClient() {
        reset();
    }

    void connect(const sockaddr* addr, int addrlen) {
        bcsocket_t sock = get();
        if (TImpl::implConnect(sock, addr, addrlen) < 0) {
            throw BCSocketException("connect failed");
        }
    }
    virtual bcsocket_t implSocket(int af, int type, int proto) {
        bcsocket_t sock = TImpl::implSocket(af, type, proto);
        return sock;
    }
    virtual void implClose(bcsocket_t sock) override {
        TImpl::implClose(sock);
    }
    virtual int implRead(void* buffer, int bufferlen, int flags) override {
        return TImpl::implRecv(mSocket, buffer, bufferlen, flags);
    }
    virtual int implWrite(const void* buffer, int bufferlen, int flags) override {
        return TImpl::implSend(mSocket, buffer, bufferlen, flags);
    }
    virtual int implSelect(bcsocket_t fdMax, fd_set *readfds, fd_set *writefds,
                           fd_set *exceptfds, struct timeval *timeout) const override {
        return TImpl::implSelect(fdMax, readfds, writefds, exceptfds, timeout);
    }
};

template<typename TImpl>
class BCSocketSrv : public BCSocketBase
{
public:
    BCSocketSrv()
    {}
    BCSocketSrv(int af, int type, int proto, bcsocket_t s)
    : BCSocketBase(af, type, proto, s)
    {}
    virtual ~BCSocketSrv() {
        BCSocketBase::reset(INVALID_SOCKET);
    }

    virtual void bind() = 0;

    void listen(int backlog) {
        if (TImpl::implListen(mSocket, backlog) < 0) {
            throw BCSocketException("listen failed");
        }
    }

    bcsocket_t accept() {
        return TImpl::implAccept(mSocket, nullptr, nullptr);
    }

    void reset(int af, int type, int proto) {
        bcsocket_t s = TImpl::implSocket(af, type, proto);
        BCSocketBase::reset(af, type, proto, s);
    }

protected:
    void implBind(const sockaddr* addr, int addrlen) {
        if (TImpl::implBind(mSocket, addr, addrlen) < 0) {
            throw BCSocketException("bind failed");
        }
    }
    int implSelect(bcsocket_t fdMax, fd_set *readfds, fd_set *writefds,
                   fd_set *exceptfds, struct timeval *timeout) const override {
        return TImpl::implSelect(fdMax, readfds, writefds, exceptfds, timeout);
    }
};

} // BCSockets
} // BCTools
