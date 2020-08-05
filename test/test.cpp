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

#include <BCSockets/BCSocketBase.hpp>

#include <gtest/gtest.h>

class BCSocketMock : public BlackCodex::BCSockets::BCSocketBase
{
    std::string mData;

    int mCountClose;
    int mCountRead;
    int mCountWrite;

    int mMaxRead;

public:
    static const bcsocket_t SOCK_ID_MOCK = 0x0FEED50C;

    BCSocketMock()
    : BCSocketBase(SOCK_ID_MOCK)
    , mMaxRead(1024)
    {}

    void setMaxRead(int mx) {
        mMaxRead = mx;
    }

protected:
    virtual void baseclose(bcsocket_t sock) override {
        (void)(sock);
        mCountClose++;
    }
    virtual int baseread(void* buffer, int bufferlen, int flags) override {
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
    virtual int basewrite(const void* buffer, int bufferlen, int flags) override {
        (void)(flags);
        mData.assign((char*)buffer, bufferlen);
        mCountWrite++;
        return mData.size();
    }
};

TEST(BCSocketBase, basic)
{
    using namespace BlackCodex::BCSockets;

    static const std::string STRING_TEST("SomeWrite");

    BCSocketBase base;
    EXPECT_EQ(INVALID_SOCKET, base.get());
    EXPECT_EQ(-1, base.write("Text"));
    EXPECT_THROW(base.read(), BCSocketException);
}

TEST(BCSocketBase, basicMock)
{
    using namespace BlackCodex::BCSockets;

    static const std::string STRING_TEST("SomeWrite");

    BCSocketMock mock;
    const bcsocket_t expected = BCSocketMock::SOCK_ID_MOCK;
    EXPECT_EQ(expected, mock.get());
    EXPECT_EQ(9, mock.write(STRING_TEST));
    
    std::string str = mock.read();
    EXPECT_EQ(STRING_TEST, str);
}

TEST(BCSocketBase, basicMockMaxRead)
{
    using namespace BlackCodex::BCSockets;

    static const std::string STRING_TEST_WRITE("SomeWrite");
    static const std::string STRING_TEST_READ ("Some");

    BCSocketMock mock;
    const bcsocket_t expected = BCSocketMock::SOCK_ID_MOCK;
    EXPECT_EQ(expected, mock.get());
    mock.setMaxRead(4);
    EXPECT_EQ(9, mock.write(STRING_TEST_WRITE));
    
    std::string str = mock.read();
    EXPECT_EQ(STRING_TEST_READ, str);
}
