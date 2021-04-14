#include <burger/base/Log.h>
#include "chatClient.h"


using namespace std::placeholders;

ChatClient::ChatClient(EventLoop* loop, const InetAddress& serverAddr) 
    : loop_(loop),
    client_(loop, serverAddr, "ChatClient"),
    winManager_(util::make_unique<WinManager>(this)),
    info_(util::make_unique<Info>()) {
    client_.setConnectionCallback(std::bind(&ChatClient::onConnection, this, _1));
    client_.setMessageCallback(std::bind(&ChatClient::onMessage, this, _1, _2, _3));
}

void ChatClient::start() {
    client_.connect();  
    winManager_->start();
}


void ChatClient::onConnection(const TcpConnectionPtr& conn) {
    std::lock_guard<std::mutex> lock(mutex_);
    if(conn->isConnected()) {
        connection_ = conn;
    } else {
        connection_.reset();
    }
}

void ChatClient::onMessage(const TcpConnectionPtr& conn, IBuffer& buf, Timestamp time) {
    std::string msg = buf.retrieveAllAsString();
    json response = json::parse(msg);
    if(response["msgid"].get<int>() == REG_MSG_ACK) {
        signupAck(response);
    } 
    else if(response["msgid"].get<int>() == LOGIN_MSG_ACK) {
        loginAck(response);
    }
    else {
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

void ChatClient::signupAck(const json& response) {
    if (response["errno"].get<int>() != 0) {
        // Sign up failed
        // todo : 错误原因可以再细化一下吗？
        std::string name = response["name"];
        std::cout << name << "Register failed!, Try again..." << std::endl;
        winManager_->signup();
    } else {
    // Sign up succeed
    std::cout << "Sign up success, your user ID is: " << response["id"]
        << ", do not forget it!" << std::endl;
        winManager_->login();
    }
}

void ChatClient::loginAck(const json& response) {
    if (response["errno"].get<int>() != 0) {
        // login failed
        // todo : 错误原因可以再细化一下吗？
        std::string errmsg = response["errmsg"];
        std::cout << errmsg << "Login failed!, Try again..." << std::endl;
        winManager_->login();
    } else {
    // Log in succeed
    std::cout << "Login success!" << std::endl;
        winManager_->mainMenu();
    }
}



