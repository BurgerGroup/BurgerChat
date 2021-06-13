#include "winManager.h"
#include "chatClient.h"
#include <burger/base/Log.h>
#include <boost/lexical_cast.hpp>  

#include "cmdLine.h"
#include "color.h"

WinManager::WinManager(ChatClient* chatClient)
    : chatClient_(chatClient),
      interface_(util::make_unique<InterFace>()),
      quit_(false) {
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
        // 如果还没登录
        if(chatClient_->logInState_ == ChatClient::LogInState::kNotLoggedIn) {
            interface_->changeHeader();
            outputMsg(">> Enter Your Choice: 1. login 2. signup 3. exit");

            std::string input = getInput();
            int choice = 0;
            try {  
                choice = boost::lexical_cast<int>(input);  
            } catch(boost::bad_lexical_cast& e) {  
                ERROR("{}", e.what());
            }
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
                    interface_.reset();
                    exit(0);
                    break;
                } 
                default: {
                    outputMsg(">> Invalid input!", "RED");
                    break;
                }
            }
        } else if(chatClient_->logInState_ == ChatClient::LogInState::kLogging) {
            // 登录界面
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
    interface_->changeHeader("Sign Up");
    outputMsg(">> Username");    
    std::string name = getInput();  // todo : 此处用sstream是否更好
    // check name validation
    if(name.size() <= 2 || name.size() > 20) {
        outputMsg("\n Name is invalid, You must input 3 to 20 characters");
        outputMsg("Please enter any key to continue", true);
        std::cin.get();
        signup();
    }
    outputMsg(">> Password");   // todo : 如何变成密文
    std::string pwd = getInput();
    // check password validation
    // todo : 更加细化密码检查
    if(pwd.size() <= 2 || pwd.size() > 20) {
        outputMsg("\n Password is invalid, You must input 3 to 20 characters");
        outputMsg("Please enter any key to continue", true);
        std::cin.get();
        signup();
    }
    json js;
    js["msgid"] = REG_MSG;
    js["name"] = name;
    js["password"] = pwd;
    std::string request = js.dump();
    TRACE("request : {}", request);
    chatClient_->send(request);
}


void WinManager::login() {
    chatClient_->setLogInState_(ChatClient::LogInState::kLogging);
    std::string idStr;
    int id = 0;
    std::string pwd;

    while(true) {
        outputMsg(">> Input Your ID number");
        idStr = getInput();
        id = atoi(idStr.c_str());
        if (id <= 0) {
            outputMsg("Invalid ID!", "RED");
            continue;
        }
        break;
    }

    outputMsg(">> Input Your Password");

    noecho();  // 输入密码不显示
    pwd = getInput();
    echo();

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
    interface_->changeHeader("Enter Your Choice (Enter 'help' to get help)");
    while(chatClient_->logInState_ == ChatClient::kLoggedIn) {

        std::string input = getInput();
        std::string action;
        std::string content;
        
        size_t end = input.find(':');
        if(end == input.npos) {
            action = input;
        }
        else {
            action = input.substr(0, end);
            content = input.substr(end + 1, input.size() - end);
        }   

        if(CmdHandler::commandMap.find(action) == CmdHandler::commandMap.end()) {
           outputMsg(">> Invalid input!!!!", "RED"); 
            
        }
        else {
            auto func = CmdHandler::commandHandlerMap[action];
            func(chatClient_, std::move(content));
        }
    }
}
