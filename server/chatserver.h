#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "burger/net/TcpServer.h"
#include "burger/net/EventLoop.h"

using namespace burger;
using namespace burger::net;

class ChatServer {
public:
    ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &name);
    void start();
private:
    void onConnection(const TcpConnectionPtr&);

    void onMessage(const TcpConnectionPtr&,
                   Buffer&,
                   Timestamp);
private:
    TcpServer server_; 
    EventLoop *loop_;  
};




#endif // CHATSERVER_H