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

#include <map>
#include <string>
#include <vector>

namespace BlackCodex {
namespace BCSockets {

class BCHttpIncompleteHeadersException : public BCSocketException
{
public:
    BCHttpIncompleteHeadersException()
        : BCSocketException("http headers incomplete")
    {}
};

class BCHttpHeadersAlreadySentException : public BCSocketException
{
public:
    BCHttpHeadersAlreadySentException()
        : BCSocketException("http headers already sent")
    {}
};

class BCHttpInvalidHeaderException : public BCSocketException
{
public:
    BCHttpInvalidHeaderException(const std::string& header)
        : BCSocketException("http header invalid: " + header)
    {}
};

class BCHttpStatusException : public BCSocketException
{
public:
    BCHttpStatusException()
    : BCSocketException("")
    {}
};
class BCHttpStatus404NotFoundException : public BCHttpStatusException
{
public:
    BCHttpStatus404NotFoundException()
    {}
};

} // BCSockets
} // BCTools
