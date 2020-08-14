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

#include "testBCSockets.h"

#include <gtest/gtest.h>

TEST(BCSocketBase, basicCreateNoParams)
{
    using namespace BlackCodex::BCSockets;

    BCSocketBase base;

    EXPECT_EQ(-1, base.get());
}
TEST(BCSocketBase, basicCreate)
{
    using namespace BlackCodex::BCSockets;

    BCSocketBase base(AF_UNIX, 2, 3, -2);
    
    EXPECT_EQ(-2, base.get());
}
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
