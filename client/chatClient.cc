#include "chatClient.h"
#include <burger/base/Log.h>
using namespace std::placeholders;

ChatClient::ChatClient(EventLoop* loop, const InetAddress& serverAddr) 
    : loop_(loop),
    client_(loop, serverAddr, "ChatClient") {
    client_.setConnectionCallback(std::bind(&ChatClient::onConnection, this, _1));
    client_.setMessageCallback(std::bind(&ChatClient::onMessage, this, _1, _2, _3));
}

void ChatClient::start() {
    for (;;) {
        std::cout << ">> 1. login 2. signup 3. exit <<" << std::endl; 
        std::cout << ">> ";
        // int choice = 0;
        // std::cin >> choice;
        // std::cin.get();
        // while (std::cin.fail()) {
        //     std::cin.clear();
        //     std::cin.ignore();
        // }
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
                client_.disconnect();
                exit(0);
                break;
            } 
            default: {
                std::cout << "Invalid input!" << std::endl;
                // printMenu("Invalid input!\n");
                break;
            }
                
        }
  }
}

void ChatClient::onConnection(const TcpConnectionPtr& conn) {
    std::lock_guard<std::mutex> lock(mutex_);
    if(conn->isConnected()) {
        connection_ = conn;
    } else {
        connection_.reset();
    }
}

void ChatClient::onMessage(const TcpConnectionPtr& conn, Buffer& buf, Timestamp time) {
    std::string msg = buf.retrieveAllAsString();
    json response = json::parse(msg);
    if(response["msgid"].get<int>() == REG_MSG_ACK) {
        signupAck(response);
    } else {
        std::cout << "other msg type" << std::endl;
    }
}

void ChatClient::send(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    if(connection_) {
        connection_->send(msg);
    } else {
        ERROR("connection deson't exist");
    }
}

void ChatClient::signup() {
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
    send(request);
}

void ChatClient::signupAck(const json& response) {
    if (response["errno"].get<int>() != 0) {
        // Sign up failed
        // todo : 错误原因可以再细化一下吗？
        std::string name = response["name"];
        std::cout << name << "Register failed!, Try again..." << std::endl;
        signup();
    } else {
    // Sign up succeed
    std::cout << "Sign up success, your user ID is: " << response["id"]
        << ", do not forget it!" << std::endl;
        login();
    }
}

void ChatClient::login() {
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



