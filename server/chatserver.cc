#include "chatserver.h"
#include "json/json.hpp"
#include "chatservice.h"

using json = nlohmann::json;
using namespace std::placeholders;

ChatServer::ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const std::string &name) 
    : server_(loop, listenAddr, name),
    loop_(loop) {
    server_.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));  // conn
    server_.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3)); // conn, buf, timestamp
    // todo : 我们该设置多大
    server_.setThreadNum(4);
}

void ChatServer::start() {
    server_.start();
}

void ChatServer::onConnection(const TcpConnectionPtr& conn) {
    if(!conn->isConnected()) {
        ChatService::getInstance().clientCloseException(conn);
        conn->shutdown();
    }
}

void ChatServer::onMessage(const TcpConnectionPtr& conn,
                   Buffer& buf,
                   Timestamp receiveTime) {
    std::string msg = buf.retrieveAllAsString();
    // std::cout << msg << std::endl;  // for test
    json js = json::parse(msg);
    // 达到的目的：完全解耦网络模块的代码和业务模块的代码
    // 通过js["msgid"] 获取=》业务handler=》conn  js  time
    auto msgHandler = ChatService::getInstance().getHandler(js["msgid"].get<int>());
    // 回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, receiveTime);
}







