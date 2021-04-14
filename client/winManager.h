#ifndef WINMANAGER_H
#define WINMANAGER_H

#include "json/json.hpp"
#include "msg.h"
using json = nlohmann::json;

class ChatClient;
class WinManager {
public:
    explicit WinManager(ChatClient* chatClient);
    ~WinManager() = default;
    void start();
    void signup();
    void login();
    void mainMenu();
private:
    ChatClient* chatClient_;
};


#endif // WINMANAGER_H