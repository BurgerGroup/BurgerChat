#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <burger/net/EventLoop.h>
#include <burger/net/TcpClient.h>
#include <burger/base/Util.h>

#include <iostream>
#include <mutex>
#include <atomic>
#include <vector>
#include <queue>
#include "json/json.hpp"
#include "msg.h"
#include "winManager.h"
#include "info.h"

using json = nlohmann::json;
using namespace burger;
using namespace burger::net;

class WinManager;
class Info;
class CmdHandler;

class ChatClient {
    friend class WinManager;
    friend class CmdHandler;
public:
    enum LogInState {
        kNotLoggedIn = 1,
        kLogging,
        kLoggedIn
    };

public:
    using MsgHandler = std::function<void(const json&)>;

    ChatClient(EventLoop* loop, const InetAddress& serverAddr);
    ~ChatClient();
    // todo for safety?
    TcpClient* getClient() { return &client_; }
    void start();
    void send(const std::string& msg);
    LogInState getLogInState() const { return logInState_; }
    std::shared_ptr<Info> getInfo() const { return info_; }

private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn, IBuffer& buf, Timestamp time);
    void handleMessage(const std::string& msg);
    void setLogInState_(LogInState state) { logInState_ = state; }

    void signupAck(const json& response);
    void loginAck(const json& response);
    void logoutAck(const json& response);
    void addFriendAck(const json& response);
    void outputMsg(const std::string& msg, const std::string& color = "GREEN", bool append = false) { winManager_->outputMsg(msg, color, append); }

private:
    EventLoop* loop_;
    TcpClient client_;
    std::mutex mutex_;
    std::atomic<LogInState> logInState_;
    TcpConnectionPtr connection_;
    std::unique_ptr<WinManager> winManager_;
    std::thread interactiveThread_;
    std::shared_ptr<Info> info_;
    std::unordered_map<int, MsgHandler> idMsgHandlerMap_;
    std::queue<json> friendRequests_;
};



#endif // CHATCLIENT_H