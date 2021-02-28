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
#include <BCSockets/http/BCHttpsClient.hpp>

#include <BCSockets/BCSocketHostAddress.hpp>
#include <BCSockets/BCSocketTls.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "BCTLSClient" << std::endl;

    if (3 != argc) {
        std::cout << std::endl;
        std::cout << "Usage: BCTLSClient <server> <uri>" << std::endl;
        std::cout << "       BCTLSClient www.iana.org /index.html" << std::endl;
        std::cout << std::endl;

        for (int i=0; i<argc; ++i) {
            std::cout << i << " " << argv[i] << std::endl;
        }
        std::cout << std::endl;
        return 0;
    }

    static const int            port = 443;
    static const std::string    method("GET");
    const std::string           server(argv[1]);
    const std::string           uri(argv[2]);

    using namespace BlackCodex::BCSockets;
    
    BCSocketsApp app;

    std::cout << std::endl;
    std::cout << "resolving [" << server << "]" << std::endl;

    BCHttpsClient client(server, port);
    const BCSocketHostAddress& hostAddr = client.getHostAddress();
    const std::string addrIP4 = hostAddr.getAddress(PF_INET);
    const std::string addrIP6 = hostAddr.getAddress(PF_INET6);
    std::cout << "          [" << hostAddr.getCanonName() << "]" <<
                           "[" << addrIP4 << "][" << addrIP6 << "]" <<
                 std::endl <<
                 std::endl;

    BCHttpResponse resp = client.query(uri, method);
    auto code = resp.getCode();
    std::cout << "Response: " << code << std::endl;

    std::cout << "Headers: " << std::endl;
    {
        for (const auto& item : resp.getHeaders()) {
            std::cout << "    " << item.first << ": " << item.second << std::endl;
        }
    }

    if (302 == code) {
        std::cout << "Location: " << resp.getHeader("Location") << std::endl;
    }

    std::cout << "Contents:" << std::endl;
    std::cout << "" << resp.getBody() << std::endl;
    std::cout << std::endl << std::endl;

    return code < 200 || code > 299;
}
