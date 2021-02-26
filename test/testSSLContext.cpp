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

#include <BCSockets/BCSocketsApp.hpp>
#include <BCSockets/ssl/BCSSLContext.hpp>
#include <BCSockets/ssl/BCSSLException.hpp>

#include <gtest/gtest.h>

TEST(TestSSLContext, contextDefault) {
    using namespace BlackCodex::BCSockets;

    BCSocketsApp app;
    BCSSLContext ctxt;
    EXPECT_FALSE(ctxt.isServer());
    EXPECT_EQ   (nullptr, ctxt.get());
}

TEST(TestSSLContext, contextServer) {
    using namespace BlackCodex::BCSockets;

    BCSocketsApp app;
    BCSSLContext ctxt;
    ctxt.reset(BCSSLMethod::BCSSLMethod_TLS23_Server);
    EXPECT_TRUE(ctxt.isServer());
    EXPECT_NE  (nullptr, ctxt.get());

    ctxt.setCertData(BCSSLCertData("", ""));    // Do not throw
    EXPECT_THROW(
        ctxt.setCertData(BCSSLCertData("no cert file", "")),
        BCSocketSSLExceptionCertFile
        );
    EXPECT_THROW(
        ctxt.setCertData(BCSSLCertData("", "no privkey file")),
        BCSocketSSLExceptionCertFile
        );
}

TEST(TestSSLContext, contextClient) {
    using namespace BlackCodex::BCSockets;

    BCSocketsApp app;
    BCSSLContext ctxt;
    ctxt.reset(BCSSLMethod::BCSSLMethod_TLS23_Client);
    EXPECT_FALSE(ctxt.isServer());
    EXPECT_NE   (nullptr, ctxt.get());
}
