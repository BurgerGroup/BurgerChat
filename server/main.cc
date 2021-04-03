#include "chatservice.h"
#include "chatserver.h"
#include <iostream>

int main(int argc, char* argv[]) {
    LOGGER;
    EventLoop loop;
    InetAddress addr("127.0.0.1", 8888);
    ChatServer server(&loop, addr, "ChatServer");

    server.start();
    loop.loop();
}