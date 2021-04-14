#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <burger/net/TcpServer.h>
#include <burger/net/EventLoop.h>
#include <functional>
#include <string>

using namespace burger;
using namespace burger::net;

class ChatServer {
public:
    ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const std::string &name);
    void start();
private:
    void onConnection(const TcpConnectionPtr&);

    void onMessage(const TcpConnectionPtr&,
                   IBuffer&,
                   Timestamp);
private:
    TcpServer server_; 
    EventLoop *loop_;  
};




#endif // CHATSERVER_H