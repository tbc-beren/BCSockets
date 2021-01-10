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
#include <BCSockets/BCSocketException.hpp>

#include <limits>
#include <string>

namespace BlackCodex {
namespace BCSockets {

class BCSocketBase
{
protected:
    bcsocket_t  mSocket;
    int         mFamily;
    int         mType;
    int         mProto;

public:
    BCSocketBase()
    : mSocket(INVALID_SOCKET)
    , mFamily(-1)
    , mType(-1)
    , mProto(-1)
    {}
    BCSocketBase(int af, int type, int proto, bcsocket_t s)
    : mSocket(INVALID_SOCKET)
    , mFamily(-1)
    , mType(-1)
    , mProto(-1)
    {
        reset(af, type, proto, s);
    }
    virtual ~BCSocketBase() {
        reset();
    }

    bcsocket_t get() const {
        return mSocket;
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

    virtual void onClose() {}

    virtual void reset(int af, int type, int proto) {
        bcsocket_t s = implSocket(af, type, proto);
        reset(af, type, proto, s);
    }
    virtual void reset(int af, int type, int proto, bcsocket_t s) {
        mFamily = af;
        mType = type;
        mProto = proto;
        if (INVALID_SOCKET != mSocket) {
            onClose();
            implClose(mSocket);
        }
        mSocket = s;
    }

    virtual void reset(bcsocket_t s = INVALID_SOCKET) {
        reset(-1, -1, -1, s);
    }

    bool canRead(struct timeval& tv) const {
        const bcsocket_t thisSocket = get();

        fd_set ready;
        FD_ZERO(&ready);
        FD_SET(thisSocket, &ready);

        int readyCount = implSelect(thisSocket+1, &ready, NULL, NULL, &tv);
        if (0 < readyCount) {
            return true;
        }
        if (FD_ISSET(thisSocket, &ready)) {
            return true;
        }
        return false;
    }
    bool canRead() const {
        struct timeval tv = {0, 0};
        return canRead(tv);
    }

    std::string read(int flags = 0) {
        static const int MAX_BUFFER_LEN = 2048;
        std::string output;
        char text[MAX_BUFFER_LEN+1];
        int len;
        do {
            len = implRead(text, MAX_BUFFER_LEN, flags);
            if (len < 0) {
                throw BCSocketException("read failed");
            }
            output.append(text, len);
        } while(len >= MAX_BUFFER_LEN);
        return output;
    }

    int write(const std::string& text, int flags = 0) {
        if (text.length() > std::numeric_limits<int>::max()) {
            throw BCSocketException("write failed: too big");
        }
        return implWrite(text.data(), static_cast<int>(text.length()), flags);
    }

protected:
    virtual bcsocket_t implSocket(int af, int type, int proto) {
        (void)(af);
        (void)(type);
        (void)(proto);
        return INVALID_SOCKET;
    }
    virtual void implClose(bcsocket_t sock) {
        (void)(sock);
    }
    virtual int implRead(void* buffer, int bufferlen, int flags) {
        (void)(buffer);
        (void)(bufferlen);
        (void)(flags);
        return -1;
    }
    virtual int implWrite(const void* buffer, int bufferlen, int flags) {
        (void)(buffer);
        (void)(bufferlen);
        (void)(flags);
        return -1;
    }
    virtual int implSelect(bcsocket_t fdMax, fd_set *readfds, fd_set *writefds,
                           fd_set *exceptfds, struct timeval *timeout) const
    {
        (void)fdMax;
        (void)readfds;
        (void)writefds;
        (void)exceptfds;
        (void)timeout;
        return -1;
    }
};

} // BCSockets
} // BCTools
