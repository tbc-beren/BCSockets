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
#include "BCSocketImplMock.hpp"

#include <BCSockets/BCServer.hpp>
#include <BCSockets/BCSocketUnix.hpp>

#include <gtest/gtest.h>

using namespace BlackCodex::BCSockets;

// TControl is the socket used for bind / listen / accept
// TSlave is the socket that results from onConnection() / accept()
template<class TControl, class TSlave>
class BCServerTestEchoEx : public BCServerBase<TControl, TSlave>
{
public:
    std::set<std::string> messages;

    BCServerTestEchoEx(std::shared_ptr<TControl> sock)
    : BCServerBase<TControl, TSlave>(sock)
    {
        BCServerBase<TControl, TSlave>::setTimeAccept(100);
    }

    virtual std::shared_ptr<TSlave> onConnection(bcsocket_t sock) {
        return  BCServerBase<TControl, TSlave>::onConnection(sock);
    }

    virtual void onInput(TSlave& sock, const std::string& data) override {
        (void)sock;
        messages.insert(data);
        sock.write("RV:"+data);
    }
};

TEST(TestStream, DISABLED_testStreamUnix) {
    using namespace BlackCodex::BCSockets;

    static const int            SOCKET_TYPE = SOCK_STREAM;
    static const std::string    SOCKET_NAME("BCSOCKETSTEST_UNIX_SOCKET");

    unlink(SOCKET_NAME.c_str());

    BCSocketsApp app;
    BCServerTestEchoEx<BCSocketUnixSrv, BCServerSocket<BCSocketImplNative>> unixSrv(
        std::make_shared<BCSocketUnixSrv>(SOCKET_TYPE, SOCKET_NAME)
        );
    unixSrv.init();

    unixSrv.start(false);

    const std::string dataSend("Message!");

    BCSocketUnix unixClient(SOCKET_TYPE);
    unixClient.connect(SOCKET_NAME);
    unixClient.write(dataSend);

    const std::string dataRecv = unixClient.read();
    EXPECT_EQ("RV:"+dataSend, dataRecv);

    std::this_thread::sleep_for( std::chrono::milliseconds(100) );

    unixSrv.stop();

    ASSERT_EQ(1UL, unixSrv.messages.size());
    EXPECT_EQ(dataSend, *unixSrv.messages.begin());
}
TEST(TestStreamMock, testStreamMock) {
    using namespace BlackCodex::BCSockets;

    static const int            SOCKET_TYPE = SOCK_STREAM;
    static const std::string    SOCKET_NAME("BCSOCKETSTEST_MOCK_SOCKET");

    BCServerTestEchoEx<BCSocketSrvMock, BCServerSocket<BCSocketImplMock>> mockSrv(
        std::make_shared<BCSocketSrvMock>(SOCKET_TYPE, SOCKET_NAME)
        );
    mockSrv.init();
    mockSrv.start(false);

    const std::string dataSend("Message!");

    BCSocketMock unixClient(SOCKET_TYPE);
    unixClient.connect(SOCKET_NAME);
    unixClient.write(dataSend);

    const std::string dataRecv = unixClient.read();
    EXPECT_EQ("RV:"+dataSend, dataRecv);

    std::this_thread::sleep_for( std::chrono::milliseconds(100) );

    unixClient.reset();
    mockSrv.stop();

    ASSERT_EQ(1UL, mockSrv.messages.size());
    EXPECT_EQ(dataSend, *mockSrv.messages.begin());

    EXPECT_EQ(0, BCSocketMockEngine::getActiveSockets());
}
