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

    idMsgHandlerMap_.insert({LOGIN_MSG_ACK, std::bind(&ChatClient::loginAck, this, _1)});
    idMsgHandlerMap_.insert({LOGOUT_MSG, std::bind(&ChatClient::logoutAck, this, _1)});
    idMsgHandlerMap_.insert({REG_MSG_ACK, std::bind(&ChatClient::signupAck, this, _1)});
    idMsgHandlerMap_.insert({ADD_FRIEND_MSG, std::bind(&ChatClient::addFriendAck, this, _1)});

    // 群组业务管理相关事件处理回调注册
    // idMsgHandlerMap_.insert({CREATE_GROUP_MSG, std::bind(&ChatClient::createGroup, this, _1)});
    // idMsgHandlerMap_.insert({ADD_GROUP_MSG, std::bind(&ChatClient::addGroup, this, _1)});
    // idMsgHandlerMap_.insert({GROUP_CHAT_MSG, std::bind(&ChatClient::groupChat, this, _1)});
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
    int msgid = response["msgid"].get<int>();
    if(msgid == ONE_CHAT_MSG) {
        parsedMsg += response["from"];
        parsedMsg += " says: ";
        parsedMsg += response["msg"];
        std::cout << BOLDBLUE << parsedMsg << std::endl;
        std::cout << RESET;
    }
    else {
        auto it = idMsgHandlerMap_.find(msgid);
        if (it == idMsgHandlerMap_.end()) {  // not find
            std::cout << BOLDBLUE << "other msg type" << std::endl;
            ERROR("msgid : {} ChatClient can't find handler", msgid);
        } else {
            auto msgHandler = idMsgHandlerMap_[msgid];
            msgHandler(std::move(response));
        }
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

void ChatClient::addFriendAck(const json& response) {
    if (response["errno"].get<int>() != 0) {
        // add failed
        // todo : 错误原因可以再细化一下吗？
        std::string errmsg = response["errmsg"];
        std::cout << RED << errmsg << "Add Friend failed!" << std::endl;
    } else {
        auto state = addFriendRequestState(response["addFriendRequestState"]);
        if (state == kApply) {
            friendRequests_.push(std::move(response));
            std::cout << YELLOW << "You have a new friend request!!!" 
                     << friendRequests_.size() << " request(s) in total." << std::endl;
            std::cout << RESET;
        }
        else if(state == kAgree) {
            std::cout << YELLOW << response["id"] << " now is your friend!" << std::endl;
            std::cout << RESET;
        }
        else if(state == kRefuse) {
            std::cout << RED << response["id"] << " refuses to be your friend!" << std::endl;
            std::cout << RESET;
        }
        else {   // what?
            std::cout << RED << "Unknown response!" << std::endl;
            std::cout << RESET;
        }   
    }
}



