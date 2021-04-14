#include "chatservice.h"
#include "chatserver.h"
#include <iostream>
#include <signal.h>
// signal 处理服务器ctrl+c，重置user的状态信息
// todo : 抽象出类? 用sigaction 
void resetHandler(int) {
    ChatService::getInstance().reset();
    exit(0);
}


int main(int argc, char* argv[]) {
    LOGGER();
    spdlog::set_level(spdlog::level::trace);
    signal(SIGINT, resetHandler);
    EventLoop loop;
    InetAddress addr("127.0.0.1", 8888);
    ChatServer server(&loop, addr, "ChatServer");

    server.start();
    loop.loop();
}