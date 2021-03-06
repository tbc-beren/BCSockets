/**
*
* The Black Codex Library: Chapter: BCSockets
*
* https://github.com/tbc-beren/BCSockets
*
* Mozilla Public License Version 2.0
* https://github.com/tbc-beren/BCSockets/blob/master/LICENSE
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* https://mozilla.org/MPL/2.0/.
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

    BCSocketBase base(AF_UNIX, SOCK_DGRAM, 3, -2);
    
    EXPECT_EQ(-2,         base.get());
    EXPECT_EQ(AF_UNIX,    base.getFamily());
    EXPECT_EQ(SOCK_DGRAM, base.getType());
    EXPECT_EQ(3,          base.getProto());
}
TEST(BCSocketBase, invalidSocket)
{
    using namespace BlackCodex::BCSockets;

    static const std::string STRING_TEST("SomeWrite");

    BCSocketBase base;
    EXPECT_EQ(INVALID_SOCKET, base.get());
    EXPECT_THROW(base.read(), BCSocketException);
    EXPECT_EQ(-1, base.write("Text"));
}
TEST(BCSocketBase, invalidSocketCanRead)
{
    using namespace BlackCodex::BCSockets;

    static const std::string STRING_TEST("SomeWrite");

    BCSocketBase base;
    EXPECT_EQ(INVALID_SOCKET, base.get());
    EXPECT_THROW(base.canRead(), BCSocketException);
}
TEST(BCSocketBase, basicMock)
{
    using namespace BlackCodex::BCSockets;

    static const std::string STRING_TEST("SomeWrite");

    BCSocketMockImpl mock;
    const bcsocket_t expected = BCSocketMockImpl::SOCK_ID_MOCK;
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

    BCSocketMockImpl mock;
    const bcsocket_t expected = BCSocketMockImpl::SOCK_ID_MOCK;
    EXPECT_EQ(expected, mock.get());
    mock.setMaxRead(4);
    EXPECT_EQ(9, mock.write(STRING_TEST_WRITE));
    
    std::string str = mock.read();
    EXPECT_EQ(STRING_TEST_READ, str);
}
