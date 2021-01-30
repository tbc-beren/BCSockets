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

#define LINE_END    "\r\n"

static const std::string    TEST_BODY       ("Body");
static const std::string    PROTOC_HTTP11   ("HTTP/1.666");
static const int            STATUS_OK       {200};
static const std::string    STATUS_MSG_OK   ("OK");

const std::string RSP_HEADERS = 
    PROTOC_HTTP11 + " 200 OK" LINE_END
    "Date: Mon, 07 Oct 2019 07:20:45 GMT" LINE_END
    "Last-Modified: Wed, 01 Sep 2004 13:24:52 GMT" LINE_END
    "ETag: \"2cc8-3e3073913b100-gzip\"" LINE_END
    "Accept-Ranges: bytes" LINE_END
    "Vary: Accept-Encoding" LINE_END
    "Content-Encoding: gzip" LINE_END
    "Cache-Control: max-age=21600" LINE_END
    "Expires: Mon, 07 Oct 2019 13:20:45 GMT" LINE_END
    "Content-Length: 3532" LINE_END
    "Content-Type: text/html; charset=iso-8859-1" LINE_END
    "Strict-Transport-Security: max-age=15552000; includeSubdomains; preload" LINE_END
    "Content-Security-Policy: upgrade-insecure-requests" LINE_END  LINE_END;

TEST(HttpResponse, testProtoCodeAndMessage)
{
    using namespace BlackCodex::BCSockets;

    const static std::string REQ_HEADERS(
            PROTOC_HTTP11 + " " + std::to_string(STATUS_OK) + " " + STATUS_MSG_OK + LINE_END
            LINE_END
    );

    BCHttpResponse ex;

    ex.feed(REQ_HEADERS);

    EXPECT_EQ(PROTOC_HTTP11, ex.getProtocol());
    EXPECT_EQ(STATUS_OK,     ex.getCode());
    EXPECT_EQ(STATUS_MSG_OK, ex.getMessage());
    EXPECT_FALSE(ex.isCompleted());
}

TEST(HttpResponse, testResponseSetCode)
{
    using namespace BlackCodex::BCSockets;

    BCHttpResponse ex;

    ex.setCode(STATUS_OK);

    EXPECT_EQ  (STATUS_OK,     ex.getCode());
    EXPECT_TRUE(ex.getMessage().empty());
}

TEST(HttpResponse, testResponseSetHeader)
{
    using namespace BlackCodex::BCSockets;

    BCHttpResponse ex;

    const char* TEST_HEADER = "x-custom-header";
    const char* TEST_HDR_VALUE = "some-test-value";
    ex.setHeader(TEST_HEADER, TEST_HDR_VALUE);

    EXPECT_EQ  (TEST_HDR_VALUE,     ex.getHeader(TEST_HEADER));
}

TEST(HttpResponse, testResponseSetHeaderOnAlreadySent)
{
    using namespace BlackCodex::BCSockets;

    BCHttpResponse ex;

    const std::string TEST_HEADER("x-custom-header");
    const std::string TEST_HDR_VALUE("some-test-value");
    const std::string TEST_EXPECTED_RESPONSE(
            "HTTP/1.1 200 " + STATUS_MSG_OK + "" LINE_END +
            TEST_HEADER + ": " + TEST_HDR_VALUE + LINE_END +
            "Content-Length: " + std::to_string(TEST_BODY.length())+ LINE_END
            LINE_END +
            TEST_BODY
        );

    ex.setHeader(TEST_HEADER, TEST_HDR_VALUE);
    ex.setCode(STATUS_OK, STATUS_MSG_OK);
    ex.send(TEST_BODY);
    
    std::string response = ex.getResponseHeaders() + ex.getBody();

    EXPECT_EQ   (TEST_HDR_VALUE,            ex.getHeader(TEST_HEADER));
    EXPECT_EQ   (TEST_BODY,                 ex.getBody());
    EXPECT_EQ   (TEST_EXPECTED_RESPONSE,    response);
    EXPECT_THROW(ex.setHeader("Some-Header", "Some-Value"), BCHttpHeadersAlreadySentException);
}
