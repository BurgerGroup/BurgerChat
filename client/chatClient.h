#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <burger/net/EventLoop.h>
#include <burger/net/TcpClient.h>
#include <iostream>
#include <mutex>
#include "json/json.hpp"
#include "msg.h"
using json = nlohmann::json;
using namespace burger;
using namespace burger::net;

class ChatClient {
public:
    ChatClient(EventLoop* loop, const InetAddress& serverAddr);
    ~ChatClient() = default;
    void connect() { client_.connect(); }
    void start();
private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn, Buffer& buf, Timestamp time);
    void send(const std::string& msg);
    void signup();
    void signupAck(const json& response);
    void login();
private:
    EventLoop* loop_;
    TcpClient client_;
    std::mutex mutex_;
    TcpConnectionPtr connection_;
};



#endif // CHATCLIENT_H