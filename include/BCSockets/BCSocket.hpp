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

#include <limits>

namespace BlackCodex {
namespace BCSockets {

class BCSocket
{
    bcsocket_t mSocket;

public:
    BCSocket()
        :mSocket(INVALID_SOCKET)
    {}

    BCSocket(bcsocket_t s)
        :mSocket(s)
    {}

    BCSocket(int af, int type, int protocol)
        : mSocket( ::socket(af, type, protocol) )
    {}

    BCSocket(const BCSocket&) = delete;
    BCSocket& operator=(const BCSocket&) = delete;

    ~BCSocket() {
        reset();
    }

    bcsocket_t get() const {
        return mSocket;
    }

    void reset(bcsocket_t s = INVALID_SOCKET) {
        if (INVALID_SOCKET != mSocket) {
            closesocket(mSocket);
        }
        mSocket = s;
    }

    void reset(int af, int type, int protocol) {
        reset(::socket(af, type, protocol));
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

    void connect(const sockaddr* addr, int addrlen) {
        if (::connect(mSocket, addr, addrlen) < 0) {
            throw BCSocketException("connect failed");
        }
    }

    virtual std::string read(int flags = 0) {
        static const int MAX_BUFFER_LEN = 433;
        std::string output;
        char text[MAX_BUFFER_LEN+1];
        int len;
        do {
            len = ::recv(mSocket, text, MAX_BUFFER_LEN, flags);
            if (len < 0) {
                throw BCSocketException("read failed");
            }
            text[len] = 0;
            output += text;
        } while(len >= MAX_BUFFER_LEN);
        return output;
    }

    virtual int write(const std::string& text, int flags = 0) {
        if (text.length() > std::numeric_limits<int>::max()) {
            throw BCSocketException("write failed: too big");
        }
        return ::send(mSocket, text.data(), static_cast<int>(text.length()), flags);
    }
};

} // BCSockets
} // BCTools
