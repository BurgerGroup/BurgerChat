#include "winManager.h"
#include "chatClient.h"

WinManager::WinManager(ChatClient* chatClient)
    : chatClient_(chatClient)  {
}

void WinManager::start() {
    for (;;) {
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
                exit(0);
                break;
            } 
            default: {
                std::cout << "Invalid input!" << std::endl;
                break;
            }
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
    int id = 0;
    char pwd[50] = {0};

    while(true) {
        std::cout << "ID number: ";
        std::cin >> id;
        std::cin.get(); // 读掉缓冲区残留的回车
        while (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
        }
        if (id <= 0) {
            std::cout << "Inlvalid ID" << std::endl;
            continue;
        }
        break;
    }
    std::cout << "Password: ";
    std::cin.getline(pwd, 50);


    // json js;
    // js["msgid"] = LOGIN_MSG;
    // js["id"] = id;
    // js["password"] = pwd;
    // string request = js.dump();

    // int len = send(clientfd, request.c_str(), strlen(request.c_str()) + 1, 0);
    // if (len == -1) {
    //     printMenu("Send request: failed!\n");
    // } else {
    //     char buffer[BUFFER_SIZE] = {0};
    //     len = recv(clientfd, buffer, BUFFER_SIZE, 0);
    //     if (len == -1) {
    //     printMenu("Receive response: failed!\n");
    //     } else {
    //     json response = json::parse(buffer);
    //     if (response["errid"].get<int>() != 0) {
    //         // Log in failed
    //         printMenu();
    //         cout << response["errmsg"].get<string>() << endl;
    //     } else {
    //         // Log in succeed
    //         // Enter main menu
    //         g_mainFlag = true;
    //         mainMenu(response);
    //     }
    //     }
    // }    
}

