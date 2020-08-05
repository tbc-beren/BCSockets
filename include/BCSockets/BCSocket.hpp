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

class BCSocketBaseNet : public BCSocketBase 
{
public:
    BCSocketBaseNet()
    {}
    BCSocketBaseNet(bcsocket_t s)
    : BCSocketBase(s)
    {}

    void connect(const sockaddr* addr, int addrlen) {
        if (::connect(mSocket, addr, addrlen) < 0) {
            throw BCSocketException("connect failed");
        }
    }
    void bind(const sockaddr* addr, int addrlen) {
        if (::bind(mSocket, addr, addrlen) < 0) {
            throw BCSocketException("bind failed");
        }
    }
    void reset(int af, int type, int protocol) {
        BCSocketBase::reset(::socket(af, type, protocol));
    }
    void listen(int backlog) {
        if (::listen(mSocket, backlog) < 0) {
            throw BCSocketException("listen failed");
        }
    }

protected:
    virtual void baseclose(bcsocket_t sock) override {
        ::closesocket(sock);
    }
    virtual int baseread(void* buffer, int bufferlen, int flags) override {
        return ::recv(mSocket, buffer, bufferlen, flags);
    }
    virtual int basewrite(const void* buffer, int bufferlen, int flags) override {
        return ::send(mSocket, buffer, bufferlen, flags);
    }
};

template<class TBase> class BCSocketImpl : public TBase
{
public:
    BCSocketImpl()
    {}
    BCSocketImpl(bcsocket_t s)
    : TBase(s)
    {}
};

class BCSocket : public BCSocketImpl<BCSocketBaseNet>
{
public:
    BCSocket()
    {}

    BCSocket(bcsocket_t s)
        : BCSocketImpl<BCSocketBaseNet>(s)
    {}

    BCSocket(int af, int type, int protocol)
        : BCSocketImpl<BCSocketBaseNet>( ::socket(af, type, protocol) )
    {}

    BCSocket(const BCSocket&) = delete;
    BCSocket& operator=(const BCSocket&) = delete;

    virtual ~BCSocket() {
        BCSocketBase::reset();
    }
};

} // BCSockets
} // BCTools
