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

#ifndef BCOPENSSL
#error Requires to build with BCOPENSSL enabled
#endif

#include <BCSockets/ssl/BCSSLError.hpp>
#include <BCSockets/ssl/BCSSLException.hpp>

#include <string>

#include <openssl/ssl.h>

namespace BlackCodex {
namespace BCSockets {

enum BCSSLMethod
{
    BCSSLMethod_None = 0,
    BCSSLMethod_TLS23_Client,
    BCSSLMethod_TLS23_Server,
};

class BCSSLCertData
{
protected:
    std::string mCertChain;
    std::string mPrivKey;

public:
    BCSSLCertData()
    {}
    BCSSLCertData(const std::string& certChain, const std::string& privKey)
    : mCertChain(certChain)
    , mPrivKey(privKey)
    {}
    const std::string& getCertChain() const {
        return mCertChain;
    }
    const std::string& getPrivateKey() const {
        return mPrivKey;
    }
};

class BCSSLContext
{
    BCSSLMethod         mSSLMethodBC;
    const SSL_METHOD*   mSSLMethod;
    SSL_CTX*            mSSLCtx;
    BCSSLCertData       mCertData;

public:
    BCSSLContext()
    : mSSLMethodBC(BCSSLMethod_None)
    , mSSLMethod(nullptr)
    , mSSLCtx(nullptr)
    {}
    virtual ~BCSSLContext() {
        reset();
    }
    bool isServer() const {
        return BCSSLMethod_TLS23_Server == mSSLMethodBC;
    }
    SSL_CTX* get() const {
        return mSSLCtx;
    }
    void reset(BCSSLMethod method = BCSSLMethod_None) {
        sslRelease();
        sslMethodInit(method);
        if (nullptr != mSSLMethod) {
            mSSLCtx = SSL_CTX_new(mSSLMethod);
            SSL_CTX_set_ecdh_auto(mSSLCtx, 1);
        }
    }
    void setCertData(const BCSSLCertData& certData) {
        mCertData = certData;
        if (!mCertData.getCertChain().empty()) {
            int rv = SSL_CTX_use_certificate_chain_file(mSSLCtx, certData.getCertChain().c_str());
            if (rv <= 0) {
                throw BCSocketSSLExceptionCertChainFile(certData.getCertChain());
            }
        }

        if (!mCertData.getPrivateKey().empty()) {
            int rv = SSL_CTX_use_PrivateKey_file(mSSLCtx, certData.getPrivateKey().c_str(), SSL_FILETYPE_PEM);
            if (rv <= 0 ) {
                throw BCSocketSSLExceptionCertPrivKeyFile(certData.getPrivateKey());
            }
        }
    }

private:
    void sslMethodInit(BCSSLMethod method) {
        mSSLMethodBC = method;
        switch(method) {
        case BCSSLMethod_TLS23_Server:  mSSLMethod = SSLv23_server_method();    break;
        case BCSSLMethod_TLS23_Client:  mSSLMethod = SSLv23_client_method();    break;
        default:
            break;
        }
    }
    void sslRelease() {
        mSSLMethod = nullptr;
        if (nullptr != mSSLCtx) {
            SSL_CTX_free(mSSLCtx);
            mSSLCtx = nullptr;
        }
    }
};

} // BCSockets
} // BCTools

