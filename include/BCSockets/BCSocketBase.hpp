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
    bcsocket_t mSocket;

public:
    BCSocketBase()
    : mSocket(INVALID_SOCKET)
    {}
    BCSocketBase(bcsocket_t s)
    : mSocket(s)
    {}

    virtual ~BCSocketBase() {
        reset();
    }

    bcsocket_t get() const {
        return mSocket;
    }

    virtual void onClose() {}

    virtual void reset(bcsocket_t s = INVALID_SOCKET) {
        if (INVALID_SOCKET != mSocket) {
            onClose();
            baseclose(mSocket);
        }
        mSocket = s;
    }

    std::string read(int flags = 0) {
        static const int MAX_BUFFER_LEN = 2048;
        std::string output;
        char text[MAX_BUFFER_LEN+1];
        int len;
        do {
            len = baseread(text, MAX_BUFFER_LEN, flags);
            if (len < 0) {
                throw BCSocketException("read failed");
            }
            text[len] = 0;
            output += text;
        } while(len >= MAX_BUFFER_LEN);
        return output;
    }

    int write(const std::string& text, int flags = 0) {
        if (text.length() > std::numeric_limits<int>::max()) {
            throw BCSocketException("write failed: too big");
        }
        return basewrite(text.data(), static_cast<int>(text.length()), flags);
    }

protected:
    virtual void baseclose(bcsocket_t sock) {
        (void)(sock);
    }
    virtual int baseread(void* buffer, int bufferlen, int flags) {
        (void)(buffer);
        (void)(bufferlen);
        (void)(flags);
        return -1;
    }
    virtual int basewrite(const void* buffer, int bufferlen, int flags) {
        (void)(buffer);
        (void)(bufferlen);
        (void)(flags);
        return -1;
    }
};

} // BCSockets
} // BCTools
