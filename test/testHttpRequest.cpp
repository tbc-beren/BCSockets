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

static const std::string METHOD_GET("GET");
static const std::string PROTOC_HTTP11("HTTP/1.1");
static const std::string URL_TEST1("/Protocols/rfc2616/rfc2616-sec5.html");

/*
const std::string REQ_HEADERS_SAMPLE = 
    METHOD_GET + " " + URL_TEST1 + " " + PROTOC_HTTP11 + LINE_END
    "User-Agent: PostmanRuntime/7.16.3" LINE_END
    "Accept: *" "/" "*" LINE_END
    "Cache-Control: no-cache" LINE_END
    "Host: www.w3.org" LINE_END
    "Accept-Encoding: gzip, deflate" LINE_END
    "Connection: keep-alive" LINE_END LINE_END;
*/

TEST(HttpRequest, testMethodUrlAndProto)
{
    using namespace BlackCodex::BCSockets;

    const static std::string REQ_HEADERS(
            METHOD_GET + " " + URL_TEST1 + " " + PROTOC_HTTP11 + LINE_END
            "User-Agent: MyUserAgent" LINE_END
            LINE_END
    );

    BCHttpRequest ex;

    ex.feed(REQ_HEADERS);

    EXPECT_EQ(METHOD_GET,    ex.getMethod());
    EXPECT_EQ(URL_TEST1,     ex.getUri());
    EXPECT_EQ(PROTOC_HTTP11, ex.getProtocol());

    EXPECT_EQ(1, ex.getHeaders().size());
}

TEST(HttpRequest, testMethodUrlAndProtoWithSpace)
{
    using namespace BlackCodex::BCSockets;

    // PLEASE NOTE THIS URL IS NOT VALID!
    // param shall be encoded as : param='some%20text'
    static const std::string URL_INVALID_WITH_SPACE("/uri/to/resource?param='some text'");

    const static std::string REQ_HEADERS(
            METHOD_GET + " " + URL_INVALID_WITH_SPACE + " " + PROTOC_HTTP11 + LINE_END
            "User-Agent: MyUserAgent" LINE_END
            "cache-control: max-age=0" LINE_END
            LINE_END
    );

    BCHttpRequest ex;

    ex.feed(REQ_HEADERS);

    EXPECT_EQ(METHOD_GET,               ex.getMethod());
    EXPECT_EQ(URL_INVALID_WITH_SPACE,   ex.getUri());
    EXPECT_EQ(PROTOC_HTTP11,            ex.getProtocol());

    EXPECT_EQ(2, ex.getHeaders().size());
}

TEST(HttpRequest, testStatusLineInvalid)
{
    using namespace BlackCodex::BCSockets;

    const static std::string REQ_HEADERS(
            LINE_END
            "User-Agent: MyUserAgent" LINE_END
            LINE_END
    );
    const static std::vector<std::string> REQ_STATUS_INVALID_TESTS = {
            METHOD_GET,                         // Method only, no spaces
            METHOD_GET + " " + PROTOC_HTTP11,   // Missing URL
    };

    std::string tmp;
    for (const auto& test : REQ_STATUS_INVALID_TESTS) {

        tmp = test + REQ_HEADERS;

        BCHttpRequest ex;
        EXPECT_THROW( ex.feed(tmp), BCHttpIncompleteHeadersException) << "URI: " << test;
    }
}

TEST(HttpRequest, testCustomOnStatus)
{
    using namespace BlackCodex::BCSockets;

    class CustomRequest : BCHttpRequest
    {
    public:
        void completeStatus(const std::vector<std::string>& statusParts) {
            onStatus(statusParts);
        }
    };

    CustomRequest customRequest;
    
    EXPECT_THROW(customRequest.completeStatus({"GET","HTTP/1.1"}), BCHttpIncompleteHeadersException);


}
