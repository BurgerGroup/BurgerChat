#include "winManager.h"
#include "chatClient.h"
#include "cmdLine.h"

WinManager::WinManager(ChatClient* chatClient)
    : chatClient_(chatClient),
      quit_(false)  {
}

WinManager::~WinManager() {
    assert(quit_ == true);
}

void WinManager::start() {
    int notifyTimes = 0;
    while(!quit_) {
        if(chatClient_->logInState_ == ChatClient::LogInState::kNotLoggedIn) {
            std::cout << ">> 1. login 2. signup 3. exit <<" << std::endl; 
            std::cout << ">> ";
            std::string input;
            std::getline(std::cin, input);
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
                    std::cout << "Bye!" << std::endl;
                    chatClient_->getClient()->disconnect();
                    quit_ = true;
                    exit(0);
                    break;
                } 
                default: {
                    std::cout << "Invalid input!" << std::endl;
                    break;
                }
            }
        }
        else if(chatClient_->logInState_ == ChatClient::LogInState::kLogging) {
            // TODO : 改成condition_variable
            if(notifyTimes < 1) {    // TODO: 如果一直都是在Logging没有返回怎么处理？
                std::cout << "Logging IN/OUT.....Please Wait......" << std::endl;
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
    std::cout << ">> Sign Up << " << std::endl;
    char name[50] = {0};
    char pwd[50] = {0};
    std::cout << "Username: ";    // todo: 此处增加判断名字合法否
    std::cin.getline(name, 50);   // todo : 此处用sstream是否更好
    std::cout << "Password: ";
    std::cin.getline(pwd, 50);

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
    // char pwd[50] = {0};
    std::string pwd;

    while(true) {
        std::cout << "ID number: ";
        // std::cin >> id;
        // std::cin.get(); // 读掉缓冲区残留的回车
        std::getline(std::cin, idStr);
        id = atoi(idStr.c_str());
        // while (std::cin.fail()) {
        //     std::cin.clear();
        //     std::cin.ignore();
        // }
        if (id <= 0) {
            std::cout << "Invalid ID" << std::endl;
            continue;
        }
        break;
    }
    std::cout << "Password: ";
    std::getline(std::cin, pwd);

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
    std::cout << ">> Main Menu <<" << std::endl; 
    std::cout << ">> Enter 'help' to get help <<" << std::endl; 
    while(chatClient_->logInState_ == ChatClient::kLoggedIn) {
        std::cout << ">> Enter Your Choice <<" << std::endl; 
        std::string input;
        std::string action;
        std::string content;
        std::getline(std::cin, input);

        size_t end = input.find(':');
        if(end == input.npos) {
            action = input;
        }
        else {
            action = input.substr(0, end);
            content = input.substr(end + 1, input.size() - end);
        }   

        if(CmdHandler::commandMap.find(action) == CmdHandler::commandMap.end()) {
            std::cout << ">> Invalid input!!!! <<" << std::endl; 
        }
        else {
            auto func = CmdHandler::commandHandlerMap[action];
            func(chatClient_, std::move(content));
        }
    }
}

