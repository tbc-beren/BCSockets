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
#include <BCSockets/http/BCHttpExchange.hpp>

#include <gtest/gtest.h>

#define TEST_BODY \
    "This is the data.\r\n" \
    "Data has several lines.\r\n" \
    "This is the last line"

TEST(HttpExchange, basic)
{
    using namespace BlackCodex::BCSockets;

    BCHttpExchange ex;

    std::map<std::string, std::string> fields = {
        { "FieldA", "value"},
        { "FieldB", "value1;value2"},
        { "FieldC", "value1a,value1b;value2a,value2b"}
    };

    static const std::string lineEnd = "\r\n";
    static const std::string status  = "REQUEST-RESPONSE STATUS LINE";

    std::string feed(status + lineEnd);
    for (const auto& field : fields) {
        feed += field.first + ": " + field.second + lineEnd;
    }

    feed += lineEnd;
    feed += TEST_BODY;

    ex.feed(feed);

    EXPECT_EQ(3, ex.getHeaders().size());
    EXPECT_STREQ(status.c_str(), ex.getStatus().c_str());
    for (const auto& field : fields) {
        EXPECT_STREQ(field.second.c_str(), ex.getHeader(field.first).c_str());
    }
    EXPECT_STREQ(TEST_BODY, ex.getBody().c_str());

    EXPECT_TRUE(ex.getHeader("NotExistingKey").empty());
}

TEST(HttpExchange, invalidHeaderException)
{
    using namespace BlackCodex::BCSockets;

    BCHttpExchange ex;

    std::string feed(
        "GET / HTTP/1.1\r\n"
        "no-colon.header\r\n"
        "\r\n"
        );

    try {
        ex.feed(feed);
        FAIL(); // Exception expected
    }
    catch(const BCHttpInvalidHeaderException& ex) {
        EXPECT_EQ(std::string("http header invalid: no-colon.header"), ex.what());
    } catch(...) {
        FAIL() << "Unexpected exception";
    }

}

TEST(HttpExchange, incompleteHeaderException)
{
    using namespace BlackCodex::BCSockets;

    std::string feed(
        "GET / HTTP/1.1\r\n"
        "content-encoding: gzip\r\n"
        );

    EXPECT_THROW(BCHttpExchange().feed(feed), BCHttpIncompleteHeadersException);
}
