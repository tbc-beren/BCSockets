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

TEST(TestMockSocket, serverBasic)
{
    using namespace BlackCodex::BCSockets;

    const std::string SOCKET_ENDPOINT("MOCKSOCKETSERVER-serverBasic-TestEndpoint");
    BCSocketSrvMock mockServer(SOCK_STREAM, SOCKET_ENDPOINT);
    mockServer.bind();

    EXPECT_THROW(mockServer.bind(), BCSocketException);     // Already binded

    // Already binded, cannot connect
    EXPECT_EQ(-1, BCSocketMockEngine::socketGet(mockServer.get()).connect(SOCKET_ENDPOINT));
    EXPECT_EQ(EADDRINUSE, BCSocketMockEngine::socketGet(mockServer.get()).implErrno());

    mockServer.listen(1);
    EXPECT_THROW(mockServer.listen(1), BCSocketException);  // Already listening

    bcsocket_t fd = INVALID_SOCKET;
    
    fd = mockServer.accept(); // TODO: Shall block?

    EXPECT_EQ(INVALID_SOCKET, fd);
}

TEST(TestMockSocket, serverAlreadyBinded)
{
    using namespace BlackCodex::BCSockets;

    const std::string SOCKET_ENDPOINT("MOCKSOCKETSERVER-TestEndpoint");
    BCSocketSrvMock mockServer(SOCK_STREAM, SOCKET_ENDPOINT);
    mockServer.bind();

    EXPECT_THROW(mockServer.bind(), BCSocketException); // Already binded

    // Already binded: Same check but a differenbt route
    EXPECT_EQ(-1, BCSocketMockEngine::socketGet(mockServer.get()).bind(SOCKET_ENDPOINT));

}
TEST(TestMockSocket, clientConnectToInvalidAddress)
{
    using namespace BlackCodex::BCSockets;

    const std::string SOCKET_ENDPOINT("MOCKSOCKETSERVER-TestEndpoint-DoesNotExist");

    BCSocketMock mockClient(SOCK_STREAM);
    EXPECT_THROW(mockClient.connect(SOCKET_ENDPOINT), BCSocketException);
}
