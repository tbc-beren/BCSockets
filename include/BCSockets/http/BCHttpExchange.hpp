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

#include <BCSockets/http/BCHttpException.hpp>

#include <map>
#include <string>
#include <vector>

namespace BlackCodex {
namespace BCSockets {

typedef std::map<std::string, std::string> BCHttpHeaders;

class BCHttpExchange
{
protected:
    std::string     mStatus;
    std::string     mProtocol;
    BCHttpHeaders   mHeaders;
    bool            mHeadersComplete;
    std::string     mBody;

public:
    BCHttpExchange()
        : mHeadersComplete(false)
    {}

    const std::string&  getStatus() const {
        return mStatus;
    }
    const std::string& getProtocol() const {
        return mProtocol;
    }
    bool headersCompleted() const {
        return mHeadersComplete;
    }

    const BCHttpHeaders& getHeaders() const {
        return mHeaders;
    }

    const std::string& getHeader(const std::string& key) const {
        static const std::string HEADER_EMPTY;
        auto it = mHeaders.find(key);
        if (mHeaders.end() != it) {
            return it->second;
        }
        return HEADER_EMPTY;
    }

    const std::string& getBody() const {
        return mBody;
    }

    void feed(const std::string& data) {
        feed(data.c_str(), data.length());
    }

    void feed(const char* data, size_t datasize) {
        if (mHeadersComplete) {
            mBody.append(data, datasize);
            return;
        }

        const char* pos = data;
        while(datasize>0 && !mHeadersComplete) {
            if (pos[0] == '\r' && pos[1] == '\n') {
                saveHeader(data, pos-data);
                data=pos+2;
                pos++;
                datasize--;
            }
            pos++;
            datasize--;
        }

        if (mHeadersComplete) {
            feed(data, datasize);
        } else {
            throw BCHttpIncompleteHeadersException();
        }
    }

protected:
    virtual void onStatusLine() {}

    void setProtocol(const std::string& protocol) {
        mProtocol = protocol;
    }

private:
    void processHeaderLine(const std::string& line) {
        size_t posColon = line.find_first_of(':');
        if (line.npos == posColon) {
            throw BCHttpInvalidHeaderException(line);
        }

        const std::string key = line.substr(0, posColon);

        posColon++;
        while (' ' == line[posColon]) {
            posColon++;
        }

        const std::string value = line.substr(posColon);
        mHeaders[key] = value;
    }
    void saveHeader(const char* data, size_t size) {
        if (size == 0) {
            mHeadersComplete = true;
        } else if (mStatus.empty()) {
            mStatus.assign(data, size);
            onStatusLine();
        } else {
            processHeaderLine(std::string(data, size));
        }
    }
};

class BCHttpRequestResponseCommon : public BCHttpExchange
{
protected:
    virtual void onStatus(const std::vector<std::string>& data) = 0;

    virtual void onStatusLine() override {
        _parseStatus(getStatus());
    }

private:
    void _parseStatus(const std::string& status) {
        std::vector<std::string> tokens;
        size_t end1 = status.find(' ');
        if (std::string::npos == end1) {
            throw BCHttpIncompleteHeadersException();
        }

        tokens.emplace_back(status.substr(0, end1));
        while(status[end1] == ' ') {
            end1++;
        }

        size_t end2 = status.rfind(' ');
        // npos will never happen, otherwise previous find() shall have returned npos
        tokens.emplace_back(status.substr(end1, end2-end1));
        while(status[end2] == ' ') {
            end2++;
        }

        if (end1 == end2) {
            // Status line cointains only one space(Two items)
            throw BCHttpIncompleteHeadersException();
        }

        tokens.emplace_back(status.substr(end2));
        onStatus(tokens);
    }
};

class BCHttpRequest : public BCHttpRequestResponseCommon
{
    std::string mMethod;
    std::string mUri;

public:
    const std::string& getMethod() const {
        return mMethod;
    }
    const std::string& getUri() const {
        return mUri;
    }

protected:
    void onStatus(const std::vector<std::string>& parts) override {
        if (3 != parts.size()) {
            throw BCHttpIncompleteHeadersException();
        }
        mMethod = parts[0];
        mUri = parts[1];
        setProtocol(parts[2]);
    }
};

class BCHttpResponse : public BCHttpRequestResponseCommon
{
    int mCode;
    std::string mMessage;

    bool mComplete;

public:
    BCHttpResponse()
        : mCode(400)
        , mMessage("Bad Request")
        , mComplete(false)
    {
        setProtocol("HTTP/1.1");
    }
    int getCode() const {
        return mCode;
    }
    const std::string& getMessage() const {
        return mMessage;
    }
    bool isCompleted() const {
        return mComplete;
    }
    void setCode(int code) {
        setCode(code, "");
    }
    void setCode(int code, const std::string& message) {
        mCode = code;
        mMessage = message;
    }
    void setHeader(const std::string& key, const std::string& value) {
        if (headersCompleted()) {
            throw BCHttpHeadersAlreadySentException();
        }
        mHeaders[key] = value;
    }
    void send(const std::string& body = std::string()) {
        sendHeaders();
        mBody = body;
        mComplete = true;
    }
    void sendHeaders() {
        if (!headersCompleted()) {
            mHeadersComplete = true;
        }
    }
    std::string getResponseHeaders() const {
        std::string response(getProtocol() + " " + std::to_string(getCode()));
        const std::string& message = getMessage();
        if (!message.empty()) {
             response += " " + message;
        }
        response += "\r\n";
        for (const auto& header : mHeaders) {
            response += header.first + ": " + header.second + "\r\n";
        }
        if (0 < mBody.length()) {
            response += "Content-Length: " + std::to_string(mBody.length()) + "\r\n";
        }

        response += "\r\n";

        return response;
    }

    void fromRequest(const BCHttpRequest& req) {
        setProtocol(req.getProtocol());
    }

protected:
    virtual void onStatus(const std::vector<std::string>& parts) override {
        setProtocol(parts[0]);
        mCode = atoi(parts[1].c_str());
        mMessage = parts[2];
    }
};

} // BCSockets
} // BCTools
