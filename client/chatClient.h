#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <burger/net/EventLoop.h>
#include <burger/net/TcpClient.h>
#include <burger/base/Util.h>

#include <iostream>
#include <mutex>
#include <atomic>
#include <vector>
#include "json/json.hpp"
#include "msg.h"
#include "winManager.h"
#include "info.h"

using json = nlohmann::json;
using namespace burger;
using namespace burger::net;

class WinManager;
class Info;
class ChatClient {
    friend class WinManager;
public:
    ChatClient(EventLoop* loop, const InetAddress& serverAddr);
    ~ChatClient() = default;
    // todo for safety?
    TcpClient* getClient() { return &client_; }
    void start();
    void send(const std::string& msg);
    std::shared_ptr<Info> getInfo() { return info_;}
    bool getLogInState() const { return logInState_; }

public:
    enum LogInState {
        kNotLoggedIn = 1,
        kLoggIng,
        kLoggedIn
    };

private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn, IBuffer& buf, Timestamp time);
    void signupAck(const json& response);
    void loginAck(const json& response);
    void setLogInState_(LogInState state) { logInState_ = state; }
private:
    EventLoop* loop_;
    TcpClient client_;
    std::mutex mutex_;
    std::atomic<LogInState> logInState_;
    TcpConnectionPtr connection_;
    std::unique_ptr<WinManager> winManager_;
    std::thread interactiveThread_;
    // std::unique_ptr<Info> info_;
    std::shared_ptr<Info> info_;
    
};



#endif // CHATCLIENT_H