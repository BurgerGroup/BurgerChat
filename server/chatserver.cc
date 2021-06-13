#include "chatserver.h"
#include "json/json.hpp"
#include "chatservice.h"

using json = nlohmann::json;
using namespace std::placeholders;

ChatServer::ChatServer(Scheduler *sched,
               const InetAddress &listenAddr,
               const std::string &name) 
    : server_(sched, listenAddr, name),
    sched_(sched) {
        
    server_.setConnectionHandler(std::bind(&ChatServer::connHandler, this, _1));  // conn
    server_.setThreadNum(4);
}

void ChatServer::start() {
    server_.start();
}


void ChatServer::connHandler(const CoTcpConnection::ptr& conn) {
    Buffer::ptr buffer = std::make_shared<Buffer>();
    while(conn->recv(buffer) > 0) {
        Timestamp receiveTime = Timestamp::now(); 
        // 这里处理string需要更加细致吗
        std::string msg = buffer->retrieveAllAsString();
        json js = json::parse(msg);
        // 达到的目的：完全解耦网络模块的代码和业务模块的代码
        // 通过js["msgid"] 获取=》业务handler=》conn  js  time
        auto msgHandler = ChatService::getInstance().getHandler(js["msgid"].get<int>());
        // 回调消息绑定好的事件处理器，来执行相应的业务处理
        msgHandler(conn, js, receiveTime);
    }
    ChatService::getInstance().clientCloseException(conn);
    conn->shutdown();
}


