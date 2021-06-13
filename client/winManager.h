#ifndef WINMANAGER_H
#define WINMANAGER_H

#include "json/json.hpp"
#include "msg.h"
#include "addFriend.h"
#include "interface.h"
#include <memory>
#include <string>
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
    void outputMsg(const std::string& msg,  bool append = false, const std::string& color = "GREEN") { interface_->outputMsg(msg, append, color); }
    void clearOutPut() { interface_->clearWin(interface_->output_); }
    std::string getInput() { return interface_->getInput(); }
    void quit() { quit_ = true; }
    
private:
    ChatClient* chatClient_;
    std::unique_ptr<InterFace> interface_;
    bool quit_;
};


#endif // WINMANAGER_H