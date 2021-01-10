/**
*
* The Black Codex Library: Chapter: BCSockets
*
* https://github.com/tbc-beren/BCSockets
*
* Mozilla Public License Version 2.0
* https://github.com/tbc-beren/BCSockets/blob/master/LICENSE
*
*/
#pragma once

#include <BCSockets/BCSocket.hpp>

#include <mutex>
#include <queue>
#include <thread>

namespace BlackCodex {
namespace BCSockets {


class BCServerSocket : public BCSocketClient<BCSocketImplEx>
{
public:
    BCServerSocket()
        : BCSocketClient<BCSocketImplEx>()
    {}
    BCServerSocket(int family, int type, int proto, bcsocket_t sock)
        : BCSocketClient<BCSocketImplEx>(family, type, proto, sock)
    {}
    /**
     * This function is called by the server whenever this connection has input available
     */
    virtual void onInput(const std::string& input) {
        (void)(input);
    }
    /**
     * This function is called by the server whenever this connection is idle
     */
    virtual void onIdle() {}

    void reset(bcsocket_t s = INVALID_SOCKET) {
        BCSocketBase::reset(s);
    }
};

template<class TMasterSocket>
class BCServer
{
    typedef std::shared_ptr<BCServerSocket> BCServerSocketPtr;

    bool                            mRun;
    int                             mTimeAccept;
    std::shared_ptr<TMasterSocket>  mSocket;
    std::thread                     mThreadMaster;
    std::thread                     mThreadHandler;
    std::mutex                      mMtxConnections;
    std::queue<BCServerSocketPtr>   mConnections;

public:
    BCServer(std::shared_ptr<TMasterSocket> sock)
       : mRun(false)
       , mTimeAccept(1000)
       , mSocket(sock)
    {}

    virtual ~BCServer()
    {}

    void setTimeAccept(int millis) {
        mTimeAccept = millis;
    }

    virtual std::shared_ptr<BCServerSocket> onConnection(bcsocket_t sock)
    {
        return std::make_shared<BCServerSocket>(
                    mSocket->getFamily(),
                    mSocket->getType(),
                    mSocket->getProto(),
                    sock
                );
    }

    virtual void onServerStart()  {}
    virtual void onServerStop()   {}
    virtual void onInput(BCServerSocket& sock, const std::string& data) {
        (void)(sock);
        (void)(data);
    }
    virtual void onPeerClose(BCServerSocket& sock) {
        (void)(sock);
    }

    void init() {
        mSocket->bind();
        mSocket->listen(5);
    }
    void start(bool bSync = true) {
        mRun = true;
        mThreadHandler = std::thread(&BCServer::_runHandlerThread, this);
        mThreadMaster = std::thread(&BCServer::_runAcceptThread, this);
        while(bSync && mRun) {
            std::this_thread::sleep_for( std::chrono::seconds(1) );
        }
    }

    void stop() {
        mRun = false;
        if(mThreadMaster.joinable()) {
            mThreadMaster.join();
        }

        if(mThreadHandler.joinable()) {
            mThreadHandler.join();
        }

        while(mConnections.size()>0) {
            mConnections.pop();
        }

        mSocket->reset();
    }

private:
    void _runHandlerThread() {
        onServerStart();
        mRun=true;
        while(mRun) {
            if (0 < mConnections.size())
            {
                std::shared_ptr<BCServerSocket> sock;
                
                {
                    std::lock_guard<std::mutex> lock(mMtxConnections);
                    sock = mConnections.front();
                    mConnections.pop();
                }

                bool forceClose = false;
                if (sock) {
                    if (INVALID_SOCKET != sock->get()) {
                        if (sock->canRead()) {
                            std::string data = sock->read();
                            if (data.empty()) {
                                onPeerClose(*sock);
                                forceClose = true;
                            } else {
                                onInput(*sock, data);
                                sock->onInput(data);
                            }
                        } else {
                            sock->onIdle();
                        }
                    } else {
                        forceClose = true;
                    }
                    if (sock->get() < 0) {
                        forceClose = true;
                    }
                    if (forceClose) {
                        sock->reset();
                        sock.reset();
                    }
                }

                if (sock) {
                    std::lock_guard<std::mutex> lock(mMtxConnections);
                    mConnections.push(sock);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        onServerStop();
    }
    void _runAcceptThread() {
        while(mRun) {
            const bcsocket_t thisSocket = mSocket->get();

            timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = mTimeAccept * 1000;
            bool can = mSocket->canRead(tv);
            if (can) {
                int sd2 = ::accept(thisSocket, NULL, NULL);
                if (sd2 < 0) {
                    continue;
                }

                std::shared_ptr<BCServerSocket> conn = onConnection(sd2) ;
                if(conn) {
                    std::lock_guard<std::mutex> lock(mMtxConnections);
                    mConnections.push(conn);
                }
            }
        }
    }
};


}
}
