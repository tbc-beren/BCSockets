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

#include <BCSockets/BCServer.hpp>
#include <BCSockets/BCSocketUnix.hpp>

#include <gtest/gtest.h>

using namespace BlackCodex::BCSockets;
template<class TMaster>
class BCServerTestEcho : public BCServer<TMaster>
{
public:
    std::set<std::string> messages;

    BCServerTestEcho(std::shared_ptr<TMaster> sock)
    : BCServer<TMaster>(sock)
    {
        BCServer<TMaster>::setTimeAccept(100);        
    }

    virtual std::shared_ptr<BCServerSocket> onConnection(bcsocket_t sock) {
        return BCServer<TMaster>::onConnection(sock);
    }

    virtual void onInput(BCServerSocket& sock, const std::string& data) override {
        (void)sock;
        messages.insert(data);
        sock.write(data);
    }
};


TEST(TestStream, DISABLED_testStreamUnix) {
    using namespace BlackCodex::BCSockets;

    static const int            SOCKET_TYPE = SOCK_STREAM;
    static const std::string    SOCKET_NAME("BCSOCKETSTEST_UNIX_SOCKET");

    unlink(SOCKET_NAME.c_str());

    BCSocketsApp app;
    BCServerTestEcho<BCSocketUnixSrv> unixSrv(
        std::make_shared<BCSocketUnixSrv>(SOCKET_TYPE, SOCKET_NAME)
        );
    unixSrv.init();

    unixSrv.start(false);

    const std::string dataSend("Message!");

    BCSocketUnix unixClient(SOCKET_TYPE);
    unixClient.connect(SOCKET_NAME);
    unixClient.write(dataSend);

    const std::string dataRecv = unixClient.read();
    EXPECT_EQ(dataSend, dataRecv);

    std::this_thread::sleep_for( std::chrono::milliseconds(100) );

    unixSrv.stop();

    ASSERT_EQ(1UL, unixSrv.messages.size());
    EXPECT_EQ(dataSend, *unixSrv.messages.begin());
}

