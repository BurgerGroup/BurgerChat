#ifndef WINMANAGER_H
#define WINMANAGER_H

#include "json/json.hpp"
#include "msg.h"
using json = nlohmann::json;

class ChatClient;
class WinManager {
public:
    explicit WinManager(ChatClient* chatClient);
    ~WinManager();
    void start();
    void signup();
    void login();
    void mainMenu();
    void quit() { quit_ = true; }
private:
    ChatClient* chatClient_;
    bool quit_;
};


#endif // WINMANAGER_H