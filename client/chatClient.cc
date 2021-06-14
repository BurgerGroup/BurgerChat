#include <burger/base/Log.h>
#include "chatClient.h"
#include "color.h"

using namespace std::placeholders;

ChatClient::ChatClient(EventLoop* loop, const InetAddress& serverAddr) 
    : loop_(loop),
    client_(loop, serverAddr, "ChatClient"),
    logInState_(kNotLoggedIn),
    winManager_(util::make_unique<WinManager>(this)),
    info_(std::make_shared<Info>()) {
    client_.setConnectionCallback(std::bind(&ChatClient::onConnection, this, _1));
    client_.setMessageCallback(std::bind(&ChatClient::onMessage, this, _1, _2, _3));

    // 绑定对应的callback
    idMsgHandlerMap_.insert({LOGIN_MSG_ACK, std::bind(&ChatClient::loginAck, this, _1)});
    idMsgHandlerMap_.insert({LOGOUT_MSG, std::bind(&ChatClient::logoutAck, this, _1)});
    idMsgHandlerMap_.insert({REG_MSG_ACK, std::bind(&ChatClient::signupAck, this, _1)});
    idMsgHandlerMap_.insert({ADD_FRIEND_MSG, std::bind(&ChatClient::addFriendAck, this, _1)});

    // 群组业务管理相关事件处理回调注册
    // idMsgHandlerMap_.insert({CREATE_GROUP_MSG, std::bind(&ChatClient::createGroup, this, _1)});
    // idMsgHandlerMap_.insert({ADD_GROUP_MSG, std::bind(&ChatClient::addGroup, this, _1)});
    // idMsgHandlerMap_.insert({GROUP_CHAT_MSG, std::bind(&ChatClient::groupChat, this, _1)});
}

ChatClient::~ChatClient()  {
    interactiveThread_.join();
}

void ChatClient::start() {
    client_.connect();  
}


void ChatClient::onConnection(const TcpConnectionPtr& conn) {
    if(conn->isConnected()) {
        // need to lock here?
        connection_ = conn;
        interactiveThread_ = std::thread(std::bind(&WinManager::start, std::ref(winManager_)));
    } else {
        // need lock here?
        connection_.reset();
    }
}

void ChatClient::onMessage(const TcpConnectionPtr& conn, IBuffer& buf, Timestamp time) {
    std::string msg = buf.retrieveAllAsString();
    handleMessage(msg);
}

void ChatClient::handleMessage(const std::string& msg) {
    json response = json::parse(msg);
    std::string parsedMsg;
    int msgid = response["msgid"].get<int>();
    if(msgid == ONE_CHAT_MSG) {   // 聊天消息  
        parsedMsg += response["from"];
        parsedMsg += " says: ";
        outputMsg(parsedMsg);
        parsedMsg = response["msg"];
        outputMsg(parsedMsg, true, "YELLOW");
    } else {
        auto it = idMsgHandlerMap_.find(msgid);
        if (it == idMsgHandlerMap_.end()) {  // not find
            outputMsg("Received Unknown type of Message!");
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
        outputMsg("connection doesn't exist");
        ERROR("connection doesn't exist");
    }
}

void ChatClient::signupAck(const json& response) {
    if (response["errno"].get<int>() != 0) {
        // Sign up failed
        // todo : 错误原因可以再细化一下吗？
        outputMsg("Register failed! Enter any key to try again...");
        std::cin.get();
        winManager_->signup();
    } else {
        // Sign up succeed
        std::string msg = "";
        msg += "Sign up success, your user ID is: ";
        msg += std::to_string(response["id"].get<UserId>());
        msg += ", do not forget it!";
        outputMsg(msg);
        outputMsg("Press a key to continue", true);
        std::cin.get();
        winManager_->login();
    }
}

void ChatClient::loginAck(const json& response) {
    if (response["errno"].get<int>() != 0) {
        // login failed
        std::string errmsg = response["errmsg"];
        winManager_->outputMsg("Login failed!");
        winManager_->outputMsg(errmsg, true);
        winManager_->outputMsg("Press any key to try again...", true);
        setLogInState_(kNotLoggedIn);
        std::cin.get();
    } else {
        // Log in succeed
        winManager_->outputMsg("Login success!");
        setLogInState_(kLoggedIn);

        auto it = response.find("offlineNotify");
        if(it != response.end()) {
            std::vector<std::string> offlineNotifications = response["offlineNotify"];
            for(const auto& req : offlineNotifications) {
                friendRequests_.push(json::parse(req));
            }
            winManager_->outputMsg("You have offline new friend requests!", "YELLOW");
        }
        
        // 设置用户信息
        info_->setName(response["name"]);
        info_->setState("online");

        it = response.find("friends");
        if(it != response.end()) {
            std::vector<std::string> friends = response["friends"];
            info_->setFriends(friends);
        }    

        // 处理离线信息
        it = response.find("offlinemsg");
        if(it != response.end()) {
            outputMsg("You have offline messages: ");
            std::vector<std::string> offlinemsgs = response["offlinemsg"];
            for(const auto& msg : offlinemsgs) { 
                handleMessage(msg);
            }
        }
    }
}

void ChatClient::logoutAck(const json& response) {
    if (response["errno"].get<int>() != 0) {
        // logout failed
        // todo : 错误原因可以再细化一下吗？
        std::string errmsg = response["errmsg"];
        outputMsg("Logout failed!, Try again...");
        setLogInState_(kLoggedIn);
    } else {
        // Log out succeed
        outputMsg("Logout success!");
        setLogInState_(kNotLoggedIn);
        info_->setState("offline");    
    }
}

void ChatClient::addFriendAck(const json& response) {
    if (response["errno"].get<int>() != 0) {
        // add failed
        // todo : 错误原因可以再细化一下吗？
        std::string errmsg = response["errmsg"];
        outputMsg("Add Friend failed!");
    } else {
        auto state = addFriendRequestState(response["addFriendRequestState"]);
        if (state == kApply) {
            friendRequests_.push(std::move(response));
            outputMsg("You have a new friend request!!!", "YELLOW");
            std::string msg = std::to_string(friendRequests_.size());
            msg += " request(s) in total.";
            outputMsg(msg, true, "YELLOW");
        }
        else if(state == kAgree) {
            std::string msg(response["friendid"]);
            msg += " now is your friend!";
            outputMsg(msg);
        }
        else if(state == kRefuse) {
            std::string msg(response["friendid"]);
            msg += " refuses to be your friend!";
            outputMsg(msg);
        }
        else {   // what?
            outputMsg("Unknown response!");
        }   
    }
}


