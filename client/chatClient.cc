#include <burger/base/Log.h>
#include "chatClient.h"
#include "color.h"

using namespace std::placeholders;

ChatClient::ChatClient(EventLoop* loop, const InetAddress& serverAddr) 
    : loop_(loop),
    client_(loop, serverAddr, "ChatClient"),
    logInState_(kNotLoggedIn),
    winManager_(util::make_unique<WinManager>(this)),
    info_(util::make_unique<Info>()) {
    client_.setConnectionCallback(std::bind(&ChatClient::onConnection, this, _1));
    client_.setMessageCallback(std::bind(&ChatClient::onMessage, this, _1, _2, _3));
}

ChatClient::~ChatClient() 
{
    interactiveThread_.join();
}

void ChatClient::start() {
    client_.connect();  
}


void ChatClient::onConnection(const TcpConnectionPtr& conn) {
    if(conn->isConnected()) {
        {   
            std::lock_guard<std::mutex> lock(mutex_);
            connection_ = conn;
        }
        interactiveThread_ = std::thread(std::bind(&WinManager::start, std::ref(winManager_)));
    } else {
        std::lock_guard<std::mutex> lock(mutex_);
        connection_.reset();
    }
}

void ChatClient::onMessage(const TcpConnectionPtr& conn, IBuffer& buf, Timestamp time) {
    std::string msg = buf.retrieveAllAsString();
    json response = json::parse(msg);
    std::string parsedMsg;
    switch (response["msgid"].get<int>())
    {
    case REG_MSG_ACK:
        signupAck(std::move(response));
        break;

    case LOGIN_MSG_ACK:
        loginAck(std::move(response));
        break;
    
    case ONE_CHAT_MSG:
        parsedMsg += response["from"];
        parsedMsg += " says: ";
        parsedMsg += response["msg"];
        std::cout << BOLDBLUE << parsedMsg << std::endl;
        std::cout << RESET;
        break;

    case LOGOUT_MSG:
        logoutAck(std::move(response));
        break;
    
    default:
        std::cout << BOLDBLUE << "other msg type" << std::endl;
        break;
    }
}

void ChatClient::send(const std::string& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    if(connection_) {
        connection_->send(msg);
    } else {
        std::cout << RED << "connection doesn't exist" << std::endl;
        ERROR("connection doesn't exist");
    }
}

void ChatClient::signupAck(const json& response) {
    if (response["errno"].get<int>() != 0) {
        // Sign up failed
        // todo : 错误原因可以再细化一下吗？
        std::string name = response["name"];
        std::cout << RED << name << "Register failed!, Try again..." << std::endl;
        winManager_->signup();
    } else {
    // Sign up succeed
    std::cout << GREEN << "Sign up success, your user ID is: " << response["id"]
        << ", do not forget it!" << std::endl;
        winManager_->login();
    }
}

void ChatClient::loginAck(const json& response) {
    if (response["errno"].get<int>() != 0) {
        // login failed
        // todo : 错误原因可以再细化一下吗？
        std::string errmsg = response["errmsg"];
        std::cout << RED << errmsg << "Login failed!, Try again..." << std::endl;
        setLogInState_(kNotLoggedIn);
    } else {
        // Log in succeed
        std::cout << GREEN << "Login success!" << std::endl;
        setLogInState_(kLoggedIn);
        
        info_->setName(response["name"]);
        info_->setState("online");    
    }
}

void ChatClient::logoutAck(const json& response) {
    if (response["errno"].get<int>() != 0) {
        // logout failed
        // todo : 错误原因可以再细化一下吗？
        std::string errmsg = response["errmsg"];
        std::cout << RED << errmsg << "Logout failed!, Try again..." << std::endl;
        setLogInState_(kLoggedIn);
    } else {
        // Log out succeed
        std::cout << GREEN << "Logout success!" << std::endl;
        setLogInState_(kNotLoggedIn);
        info_->setState("offline");    
    }
}



