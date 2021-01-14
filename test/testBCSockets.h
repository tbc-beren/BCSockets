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

#include <BCSockets/BCSocketBase.hpp>

#include <cstring>
#include <string>

class BCSocketMockImpl : public BlackCodex::BCSockets::BCSocketBase
{
    std::string mData;

    int mCountClose;
    int mCountRead;
    int mCountWrite;

    int mMaxRead;

public:
    static const bcsocket_t SOCK_ID_MOCK = 0x0FEED50C;

    BCSocketMockImpl()
    : BCSocketBase(63000, 63001, 63002, SOCK_ID_MOCK)
    , mMaxRead(1024)
    {}

    void setMaxRead(int mx) {
        mMaxRead = mx;
    }

protected:
    virtual void implClose(bcsocket_t sock) override {
        (void)(sock);
        mCountClose++;
    }
    virtual int implRead(void* buffer, int bufferlen, int flags) override {
        (void)(buffer);
        (void)(bufferlen);
        (void)(flags);

        mCountRead++;
        if (mData.size() == 0 || bufferlen == 0) {
            return -1;
        }

        int maxSize = std::min(mMaxRead, bufferlen);
        int readed = std::min((int)mData.size(), maxSize);
        memcpy(buffer, mData.data(), readed);
        mData = mData.substr(readed);
        return readed;
    }
    virtual int implWrite(const void* buffer, int bufferlen, int flags) override {
        (void)(flags);
        mData.assign((char*)buffer, bufferlen);
        mCountWrite++;
        return mData.size();
    }
    virtual int implSelect(bcsocket_t fdMax, fd_set *readfds, fd_set *writefds,
                           fd_set *exceptfds, struct timeval *timeout) const override {
        (void)fdMax;
        (void)readfds;
        (void)writefds;
        (void)exceptfds;
        (void)timeout;
        return 0;
    }
};
