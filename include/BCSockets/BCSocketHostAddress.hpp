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

#include <BCSockets/BCSocketsApp.hpp>

#include <cstring>

#include <map>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <arpa/inet.h>      // inet_ntop

namespace BlackCodex {
namespace BCSockets {

class BCSocketHostAddress
{
    std::string                 mHostName;
    std::string                 mHostCanon;
    std::map<int, std::string>  mAddressList;
    const std::string           mAddressEmpty;

public:
    const std::string& getCanonName() const {
        return mHostCanon;
    }
    const std::string& getAddress(int type) const {
        auto it = mAddressList.find(type);
        return mAddressList.end() == it? mAddressEmpty : it->second;
    }
    void resolve(const std::string& hostName) {

        struct addrinfo hints;
        memset (&hints, 0, sizeof (hints));
        hints.ai_family = PF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags |= AI_CANONNAME;

        struct addrinfo* addr = nullptr;
        struct addrinfo* res = nullptr;
        int rv = getaddrinfo(hostName.c_str(), nullptr, &hints, &addr);
        if (0 == rv) {
            mHostName = hostName;
            mHostCanon = addr->ai_canonname;
            mAddressList.clear();

            static const int MAX_ADDR_SIZE = 100;
            char addrstr[MAX_ADDR_SIZE];
            void *ptr;

            res = addr;
            while (res)
            {
                inet_ntop(res->ai_family, res->ai_addr->sa_data, addrstr, MAX_ADDR_SIZE);

                switch (res->ai_family) {
                case AF_INET:
                    ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
                    break;
                case AF_INET6:
                    ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
                    break;
                default:
                    ptr = nullptr;
                }
                if (nullptr != ptr) {
                    inet_ntop (res->ai_family, ptr, addrstr, MAX_ADDR_SIZE);
                    mAddressList[res->ai_family] = addrstr;
                }
                res = res->ai_next;
            }
        }

        if (nullptr != addr) {
            freeaddrinfo(addr);
        }
    }
};

} // BCSockets
} // BCTools
