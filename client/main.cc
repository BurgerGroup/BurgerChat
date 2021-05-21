#include "chatClient.h"
#include <ncurses.h>
#include <locale.h>
#include <burger/net/EventLoopThread.h>

int main() {
    setlocale(LC_ALL,"");  // 使ncurses支持中文
    ONLY_TO_FILE;
    LOGGER("./logs/client.txt");
    LOG_LEVEL_DEBUG;
    InetAddress serverAddr("127.0.0.1", 8888);

    EventLoop loop;
    ChatClient client(&loop, serverAddr); // todo : 这里要用startLoop -- 不然主线程一直在loop

    // EventLoopThread loopThread;
    // ChatClient client(loopThread.startLoop(), serverAddr);

    client.start();
    loop.loop();
}
