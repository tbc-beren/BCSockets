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

#include <BCSockets/BCSocketException.hpp>
#include <BCSockets/ssl/BCSSLError.hpp>

namespace BlackCodex {
namespace BCSockets {

class BCSocketSSLException : public BCSocketException
{
public:
    BCSocketSSLException(const char* message)
        : BCSocketException(message)
    {}
    BCSocketSSLException(const std::string& message)
        : BCSocketException(message)
    {}
};

class BCSocketSSLExceptionConnect : public BCSocketSSLException
{
public:
    BCSocketSSLExceptionConnect()
        : BCSocketSSLException("ssl connect failed")
    {}
};

class BCSocketSSLExceptionCertFile : public BCSocketSSLException
{
    std::string     mCertType;
    std::string     mCertFile;
public:
    BCSocketSSLExceptionCertFile(const std::string& certType, const std::string& certFile, const std::string& message)
        : BCSocketSSLException("ssl cert file error: " + certType + " - " + certFile +": " + message)
        , mCertType(certType)
        , mCertFile(certFile)
    {}
};

class BCSocketSSLExceptionCertChainFile : public BCSocketSSLExceptionCertFile
{
public:
    BCSocketSSLExceptionCertChainFile(const std::string& certFile)
        : BCSocketSSLExceptionCertFile("certchain", certFile, BCSSLError::getLastSSLError())
    {}
};

class BCSocketSSLExceptionCertPrivKeyFile : public BCSocketSSLExceptionCertFile
{
public:
    BCSocketSSLExceptionCertPrivKeyFile(const std::string& certFile)
        : BCSocketSSLExceptionCertFile("privkey", certFile, BCSSLError::getLastSSLError())
    {}
};

} // BCSockets
} // BlackCodex

