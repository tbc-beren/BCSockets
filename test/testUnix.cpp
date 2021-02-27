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

#include <BCSockets/BCSocketUnix.hpp>

#include <gtest/gtest.h>

TEST(BCSocketUnix, addrFromName) {

    static const size_t DATA_SIZE = 200;
    static const unsigned char GUARD_BYTE = 0xFF;

    std::vector<unsigned char> data;
    data.resize(DATA_SIZE);
    memset(data.data(), GUARD_BYTE, data.size());

    struct sockaddr_un* addr = (struct sockaddr_un*)data.data();
    static const size_t ADDR_PATH_SIZE = sizeof(addr->sun_path);

    std::string socketName("TestSocketName");

    // Push too the limit, but do not cross boundary
    while(socketName.size() < ADDR_PATH_SIZE - 1) {
        socketName += "_";
    }

    BlackCodex::BCSockets::BCSocketUnix::addrFromName(*addr, socketName);
    EXPECT_STRCASEEQ(socketName.c_str(), addr->sun_path);

    EXPECT_EQ(data[DATA_SIZE-1], GUARD_BYTE);
    EXPECT_EQ(addr->sun_path[ADDR_PATH_SIZE-2], '_');
    EXPECT_EQ(addr->sun_path[ADDR_PATH_SIZE-1], 0);

    unsigned char val(addr->sun_path[ADDR_PATH_SIZE]);
    EXPECT_EQ(val, GUARD_BYTE);
}

TEST(BCSocketUnix, addrFromNameTooLong) {

    struct sockaddr_un addr;
    size_t maxlen = sizeof(addr.sun_path);
    std::string socketName("TestSocketName_TooLong");
    while(socketName.size() < maxlen) {
        socketName += "_";
    }

    EXPECT_THROW(
        BlackCodex::BCSockets::BCSocketUnix::addrFromName(addr, socketName),
        BlackCodex::BCSockets::BCSocketException
    );
}
