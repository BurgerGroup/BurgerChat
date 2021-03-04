#include "chatserver.h"
#include "json/json.hpp"
#include <functional>
#include <string>
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &name) 
    : server_(loop, listenAddr, name),
    loop_(loop) {
    server_.setConnectionCallback(std::bind(&ChatServer::onConnection, this, 
                                    std::placeholder::_1));  // conn
    server_.setMessageCallback(std::bind(&ChatServer::onMessage, this, 
                                    std::placeholder::_1,   // conn 
                                    std::placeholder::_2,   // buffer
                                    std::placeholder::_3)); // timestamp
    // todo : 我们该设置多大
    server_.setThreadNum(4);
}

void ChatServer::start() {
    server_.start();
}

void ChatServer::onConnection(const TcpConnectionPtr& conn) {
    std::cout <<

    // if(!conn->isConnected()) {
        // ChatService::instance()->
    // }
}





