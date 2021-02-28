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
#pragma once

#include <BCSockets/BCSocket.hpp>
#include <BCSockets/native/BCSocketImplNative.hpp>
#include <BCSockets/ssl/BCSSLContext.hpp>
#include <BCSockets/ssl/BCSSLError.hpp>
#include <BCSockets/ssl/BCSSLException.hpp>

#include <memory>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <openssl/x509.h>
#include <openssl/x509v3.h>

namespace BlackCodex {
namespace BCSockets {

class BCSocketImplSSL
{
    SSL*                            mSSL;
    std::shared_ptr<BCSSLContext>   mSSLContext;

    BCSocketImplSSL& operator=(const BCSocketImplSSL&) = delete;
    BCSocketImplSSL(const BCSocketImplSSL&) = delete;

public:
    BCSocketImplSSL()
    : mSSL(nullptr)
    {}
    virtual ~BCSocketImplSSL() {
        sslClose();
    }

    void setContext(std::shared_ptr<BCSSLContext> SSLContext) {
        sslClose();

        mSSLContext = SSLContext;
        mSSL = SSL_new(mSSLContext->get());
    }
    std::shared_ptr<BCSSLContext> getContext() const {
        return mSSLContext;
    }
    SSL* getSSL() const {
        return mSSL;
    }
    int getSSLHandle() const {
        return SSL_get_fd(mSSL);
    }
    void sslAttach(bcsocket_t s) {
        SSL_set_fd(mSSL, s);
        if (mSSLContext->isServer()) {
            int rv = SSL_accept(mSSL);
            if (rv == -1) {
                throw BCSocketSSLException("ssl accept failed");
            }
        }
    }
    int sslRead(void* buffer, int bufferlen) {
        if (nullptr == mSSL) {
            return -1;
        }
        int readed = SSL_read(mSSL, buffer, bufferlen);
        return readed;
    }
    int sslWrite(const void* buffer, int bufferlen) {
        int len = SSL_write(mSSL, buffer, bufferlen);
        if (len < 0) {
            int err = SSL_get_error(mSSL, len);
            switch (err) {
            case SSL_ERROR_WANT_WRITE:
            case SSL_ERROR_WANT_READ:
                len = 0;
                break;
            case SSL_ERROR_ZERO_RETURN:
            case SSL_ERROR_SYSCALL:
            case SSL_ERROR_SSL:
            default:
                len = -1;
            }
        }
        return len;
    }
    void sslClose() {
        mSSLContext.reset();
        if (nullptr != mSSL) {
            SSL_shutdown(mSSL);
            SSL_free(mSSL);
            mSSL = nullptr;
        }
    }
};

class BCSocketClientTls : public BCSocketClient<BCSocketImplNative>
{
    BCSocketImplSSL mSsl;

public:
    BCSocketClientTls()
    {}
    BCSocketClientTls(int af, int type, int proto)
    {
        reset(af, type, proto);
        if (INVALID_SOCKET != get()) {
            auto ctxtPtr = std::make_shared<BCSSLContext>();
            ctxtPtr->reset(BCSSLMethod_TLS23_Client);
            mSsl.setContext(ctxtPtr);
            mSsl.sslAttach(get());
        }
    }
    virtual ~BCSocketClientTls() {
        reset();
    }
    SSL* getSSL() const {
        return mSsl.getSSL();
    }
    std::shared_ptr<BCSSLContext> getSSLContext() const {
        return mSsl.getContext();
    }
    void connect(const sockaddr* addr, int addrlen) {
        bcsocket_t sock = get();
        if (BCSocketImplNative::implConnect(sock, addr, addrlen) < 0) {
            throw BCSocketException("connect failed");
        }
        int err = SSL_connect(mSsl.getSSL());
        if (err <= 0) {
            throw BCSocketSSLExceptionConnect();
        }
    }
    virtual int implRead(void* buffer, int bufferlen, int flags) override {
        (void)(flags);
        return mSsl.sslRead(buffer, bufferlen);
    }
    virtual int implWrite(const void* buffer, int bufferlen, int flags) override {
        (void)(flags);
        return mSsl.sslWrite(buffer, bufferlen);
    }
    virtual void implClose(bcsocket_t sock) override {
        mSsl.sslClose();
        BCSocketClient<BCSocketImplNative>::implClose(sock);
    }
};

} // BCSockets
} // BCTools
