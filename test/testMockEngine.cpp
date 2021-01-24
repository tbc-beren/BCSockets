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

#include <BCSockets/BCServer.hpp>
#include <BCSockets/BCSocketUnix.hpp>
#include "BCSocketImplMock.hpp"

#include <gtest/gtest.h>

TEST(TestMockSocketEngine, bind)
{
    using namespace BlackCodex::BCSockets;

    const std::string SOCKET_ENDPOINT("MOCKSOCKETSERVER-TestEndpoint");
    BCSocketSrvMock mockServer(SOCK_STREAM, SOCKET_ENDPOINT);
    mockServer.bind();

    EXPECT_THROW(mockServer.bind(), BCSocketException); // Already binded
    EXPECT_EQ(EADDRINUSE, BCSocketMockEngine::implErrno());

    BCSocketSrvMock mockServer2(SOCK_STREAM, SOCKET_ENDPOINT);
    EXPECT_THROW(mockServer2.bind(), BCSocketException);
}
TEST(TestMockSocketEngine, selectUnsupported)
{
    using namespace BlackCodex::BCSockets;

    struct timeval  tv;
    fd_set          fdset;
    EXPECT_THROW(BCSocketMockEngine::implSelect(1, nullptr, &fdset, nullptr, &tv), BCSocketMockException);
    EXPECT_THROW(BCSocketMockEngine::implSelect(1, nullptr, nullptr, &fdset, &tv), BCSocketMockException);
}
TEST(TestMockSocketEngine, closeNotExistingSocket)
{
    using namespace BlackCodex::BCSockets;

    const bcsocket_t socketTest = 12345; // Random id... expected to no exist
    ASSERT_THROW(BCSocketMockEngine::socketGet(socketTest), BCSocketMockException);
    EXPECT_THROW(BCSocketMockEngine::implClose(socketTest), BCSocketMockException);
}
TEST(TestMockSocketEngine, peerNameFromInvalidAddress)
{
    using namespace BlackCodex::BCSockets;

    sockaddr addr;
    addr.sa_family = AF_INET; // Address family invalid for MOCK

    const std::string SOCKET_ENDPOINT("MOCKSOCKETSERVER-TestEndpoint");
    BCSocketMock mock(SOCK_STREAM);
    EXPECT_THROW(BCSocketMockEngine::implConnect(mock.get(), &addr, sizeof(addr)), BCSocketMockException);
}
TEST(TestMockSocketEngine, receiveConnectionInvalid)
{
    using namespace BlackCodex::BCSockets;

    const std::string SOCKET_ENDPOINT("MOCKSOCKETSERVER-TestEndpoint");
    BCSocketMock mock1(SOCK_STREAM);

    BCSocketMock mock2(SOCK_STREAM);

    auto& sockObject1 = BCSocketMockEngine::socketGet(mock1.get());
    auto& sockObject2 = BCSocketMockEngine::socketGet(mock2.get());

    // close sockObject2  
    sockObject2.reset();

    EXPECT_EQ(-1, sockObject1.receiveConnection(sockObject2));
 }