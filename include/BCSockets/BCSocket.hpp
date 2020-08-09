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

class BCSocketBaseImpl : public BCSocketBase 
{
    int mFamily;
    int mType;
    int mProto;

public:
    BCSocketBaseImpl()
    : mFamily(-1)
    , mType(-1)
    , mProto(-1)
    {}
    BCSocketBaseImpl(bcsocket_t s, int af, int type, int proto)
    : mFamily(-1)
    , mType(-1)
    , mProto(-1)
    {
        reset(s, af, type, proto);
    }
    BCSocketBaseImpl(int af, int type, int proto)
    : mFamily(-1)
    , mType(-1)
    , mProto(-1)
    {
        reset(af, type, proto);
    }
    int getFamily() const {
        return mFamily;
    }
    int getType() const {
        return mType;
    }
    int getProto() const {
        return mProto;
    }
    void reset(bcsocket_t s, int af, int type, int proto) {
        BCSocketBase::reset(s);
        mFamily = af;
        mType = type;
        mProto = proto;
    }
    void reset(int af, int type, int proto) {
        reset(::socket(af, type, proto), af, type, proto);
    }
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
    BCSocketImpl(bcsocket_t s, int af, int type, int proto)
        : TBase(s, af, type, proto)
    {}
    BCSocketImpl(int af, int type, int proto)
        : TBase(af, type, proto)
    {}
};

class BCSocket : public BCSocketImpl<BCSocketBaseImpl>
{
public:
    BCSocket()
    {}
    BCSocket(int af, int type, int proto)
        : BCSocketImpl<BCSocketBaseImpl>(af, type, proto)
    {}

    BCSocket(const BCSocket&) = delete;
    BCSocket& operator=(const BCSocket&) = delete;

    virtual ~BCSocket() {
        BCSocketBase::reset();
    }
};

} // BCSockets
} // BCTools
