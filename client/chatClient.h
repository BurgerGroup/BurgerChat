#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <burger/net/EventLoop.h>
#include <burger/net/TcpClient.h>
#include <burger/base/Util.h>

#include <iostream>
#include <mutex>
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
public:
    ChatClient(EventLoop* loop, const InetAddress& serverAddr);
    ~ChatClient() = default;
    // todo for safety?
    TcpClient* getClient() { return &client_; }
    void start();
    void send(const std::string& msg);
    std::shared_ptr<Info> getInfo() { return info_;}

private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn, IBuffer& buf, Timestamp time);
    void signupAck(const json& response);
    void loginAck(const json& response);
private:
    EventLoop* loop_;
    TcpClient client_;
    std::mutex mutex_;
    TcpConnectionPtr connection_;
    std::unique_ptr<WinManager> winManager_;
    // std::unique_ptr<Info> info_;
    std::shared_ptr<Info> info_;
    
};



#endif // CHATCLIENT_H