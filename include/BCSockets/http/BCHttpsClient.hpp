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

#ifndef BCSOCKETS_OPENSSL
#error Requires to build with BCSOCKETS_OPENSSL enabled
#endif

#include <BCSockets/BCSocketHostAddress.hpp>
#include <BCSockets/BCSocketTls.hpp>
#include <BCSockets/http/BCHttpExchange.hpp>
#include <BCSockets/ssl/BCSSLError.hpp>

#include <openssl/ssl.h>

namespace BlackCodex {
namespace BCSockets {

class BCHttpsClient
{
    int                 mPort;
    std::string         mHostName;
    BCSocketHostAddress mHostAddress;

public:
    BCHttpsClient(const std::string& name, int port)
    : mPort(port)
    , mHostName(name)
    {
        mHostAddress.resolve(mHostName);
    }
    const BCSocketHostAddress& getHostAddress() const {
        return mHostAddress;
    }
    BCHttpResponse query(const std::string& uri,
                         const std::string& method="GET") {

        const std::string hostIP = getHostAddress().getAddress(PF_INET);

        BCSocketClientTls sockSSL(AF_INET, SOCK_STREAM, 0);

        SSL* ssl = sockSSL.getSSL();

        struct sockaddr_in sa;
        memset (&sa, 0, sizeof(sa));
        sa.sin_family      = AF_INET;
        sa.sin_addr.s_addr = inet_addr(hostIP.c_str());
        sa.sin_port        = htons (mPort); 
        socklen_t socklen  = sizeof(sa);

        int err = SSL_set_tlsext_host_name(ssl, mHostName.c_str());
        if(0 >= err) {
            throw BCSocketSSLExceptionConnect();
        }

        sockSSL.connect((struct sockaddr *)&sa, socklen);

        const std::string version = "1.0";
        const std::string request = method + " " + uri + " HTTP/" + version + "\r\n"
                                    "Host: " + mHostName + "\r\n"
                                    "User-Agent: BCTLSClient" +
                                    "\r\n\r\n";
        // Send request
        sockSSL.write(request.c_str());

        // Receive response
        BCHttpResponse resp;
        RecvPacket(sockSSL, resp);
        return resp;
    }

    int RecvPacket(BCSocketClientTls& sock, BCHttpResponse& rsp)
    {
        int len=0;
        std::string data;

        do {
            data = sock.read();
            int len=data.size();
            int err = SSL_get_error(sock.getSSL(), len);
            if (err == SSL_ERROR_WANT_READ) {
                    return 0;
            }
            rsp.feed(data);
            // Body can be 0 for "HEAD" Request
        } while (data.size() > 0);
        if (len < 0) {
            int err = SSL_get_error(sock.getSSL(), 0);
            if (err == SSL_ERROR_WANT_READ)
                return 0;
            if (err == SSL_ERROR_WANT_WRITE)
                return 0;
            if (err == SSL_ERROR_ZERO_RETURN || err == SSL_ERROR_SYSCALL || err == SSL_ERROR_SSL)
                return -1;
        }
        return 0;
    }
};

} // BCSockets
} // BCTools
