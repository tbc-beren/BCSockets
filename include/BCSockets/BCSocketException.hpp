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

#include <stdexcept>

namespace BlackCodex {
namespace BCSockets {

class BCSocketException : public std::runtime_error
{
public:
    BCSocketException(const char* message)
        : runtime_error(message)
    {}
    BCSocketException(const std::string& message)
        : runtime_error(message)
    {}
};

class BCSocketInvalidException : public BCSocketException
{
public:
    BCSocketInvalidException()
        : BCSocketException("invalid socket")
    {}
};

class BCSocketTimeoutException : public BCSocketException
{
public:
    BCSocketTimeoutException()
        : BCSocketException("socket timeout")
    {}
};

} // BCSockets
} // BlackCodex

