#include "winManager.h"
#include "chatClient.h"
#include <burger/base/Log.h>
#include "cmdLine.h"
#include "color.h"

WinManager::WinManager(ChatClient* chatClient)
    : chatClient_(chatClient),
      interface_(util::make_unique<InterFace>()),
      quit_(false)  {
    DEBUG("WinManager ctor.");
    interface_->runHeader();
    interface_->runInput();
    interface_->runOutput();
}

WinManager::~WinManager() {
    assert(quit_ == true);
}

void WinManager::start() {
    int notifyTimes = 0;
    while(!quit_) {
        if(chatClient_->logInState_ == ChatClient::LogInState::kNotLoggedIn) {
            outputMsg(">> Enter Your Choice: 1. login 2. signup 3. exit");

            std::string input = getInput();
            int choice = atoi(input.c_str());
            // todo : 解决缓冲区问题
            switch (choice) {
                case 1: {
                    login();
                    break;
                }
                case 2: {
                    signup();
                    break;
                }
                case 3: {
                    outputMsg(">> Bye!");
                    chatClient_->getClient()->disconnect();
                    quit_ = true;
                    exit(0);
                    break;
                } 
                default: {
                    outputMsg(">> Invalid input!");
                    break;
                }
            }
        }
        else if(chatClient_->logInState_ == ChatClient::LogInState::kLogging) {
            // TODO : 改成condition_variable
            if(notifyTimes < 1) {    // TODO: 如果一直都是在Logging没有返回怎么处理？
                outputMsg(">> Logging IN/OUT.....Please Wait......");
                ++notifyTimes;
            }
        }
        else {
            notifyTimes = 0;
            mainMenu();
        }
    }
}

void WinManager::signup() {
    outputMsg(">> Sign Up ");
    outputMsg(">> Username");    // todo: 此处增加判断名字合法否
    std::string name = getInput();  // todo : 此处用sstream是否更好
    outputMsg(">> Password");
    std::string pwd = getInput();

    json js;
    js["msgid"] = REG_MSG;
    js["name"] = name;
    js["password"] = pwd;
    std::string request = js.dump();
    // std::cout << request << std::endl; // for test
    chatClient_->send(request);
}


void WinManager::login() {
    chatClient_->setLogInState_(ChatClient::LogInState::kLogging);
    std::string idStr;
    int id = 0;
    std::string pwd;

    while(true) {
        outputMsg(">> Input Your ID number");
        // std::cin >> id;
        // std::cin.get(); // 读掉缓冲区残留的回车
        // std::getline(std::cin, idStr);
        idStr = getInput();
        id = atoi(idStr.c_str());
        // while (std::cin.fail()) {
        //     std::cin.clear();
        //     std::cin.ignore();
        // }
        if (id <= 0) {
            std::cout << RED << "Invalid ID!" << std::endl;
            continue;
        }
        break;
    }
    // outputMsg("Password: ";
    // 
    outputMsg(">> Input Your Password");
    // std::getline(std::cin, pwd);
    pwd = getInput();

    chatClient_->info_->setId(id);
    chatClient_->info_->setPwd(pwd);

    json js;
    js["msgid"] = LOGIN_MSG;
    js["id"] = id;
    js["password"] = pwd.c_str();
    std::string request = js.dump();
    // std::cout << request << std::endl; // for test
    chatClient_->send(request);
}

void WinManager::mainMenu() {
    while(chatClient_->logInState_ == ChatClient::kLoggedIn) {
        outputMsg(">> Enter Your Choice (Enter 'help' to get help)");

        std::string input = getInput();
        std::string action;
        std::string content;
        
        // std::getline(std::cin, input);

        size_t end = input.find(':');
        if(end == input.npos) {
            action = input;
        }
        else {
            action = input.substr(0, end);
            content = input.substr(end + 1, input.size() - end);
        }   

        if(CmdHandler::commandMap.find(action) == CmdHandler::commandMap.end()) {
            std::cout << RED << ">> Invalid input!!!! <<" << std::endl; 
            
        }
        else {
            auto func = CmdHandler::commandHandlerMap[action];
            func(chatClient_, std::move(content));
        }
    }
}
