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

#include <string>

#include <openssl/ssl.h>
#include <openssl/err.h>

namespace BlackCodex {
namespace BCSockets {

class BCSSLError
{
public:
    static
    std::string getLastSSLError() {
        BIO *bio = BIO_new(BIO_s_mem());
        ERR_print_errors(bio);
        char *buf;
        size_t len = BIO_get_mem_data(bio, &buf);
        std::string ret(buf, len);
        BIO_free(bio);
        return ret;
    }
};

} // BCSockets
} // BCTools
