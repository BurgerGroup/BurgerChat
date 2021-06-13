#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <burger/net/CoTcpServer.h>
#include <burger/net/Scheduler.h>
#include <burger/net/Buffer.h>
#include <functional>
#include <string>

using namespace burger;
using namespace burger::net;

class ChatServer {
public:
    ChatServer(Scheduler *sched,
               const InetAddress &listenAddr,
               const std::string &name);
    void start();
private:
    void connHandler(const CoTcpConnection::ptr& conn);

private:
    CoTcpServer server_; 
    Scheduler *sched_;  
};

#endif // CHATSERVER_H