#include "chatservice.h"
#include <burger/base/Log.h>
#include <burger/base/Timestamp.h>

using namespace std::placeholders;

ChatService& ChatService::getInstance() {
    static ChatService service;
    return service;
}

// id password
// errno 
// 0. 成功
// 1. 重复登录 This account is using, input another!
// 2. User id 不存在
// 3. 密码不正确
void ChatService::login(const CoTcpConnection::ptr &conn, json &js, Timestamp time) {
    UserId id = js["id"].get<UserId>();
    std::string pwd = js["password"];
    User user = userManager_.query(id);

    if(user.getId() == id && user.getPwd() == pwd) {
        if(user.getState() == "online") {
            // 该用户已经登录，不允许重复登录 
            // todo 把挤下线的功能
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 1;
            response["errmsg"] = "This account is using, input another!";
            conn->send(response.dump());
        } else {
            // login successfully
            {
                // STL 都未考虑线程安全， onMessage在多线程环境中
                // 登录成功，记录用户连接信息
                std::lock_guard<std::mutex> lock(mutex_);
                idUserConnMap_.insert({id, conn});
            }
            // 登录成功，更新用户状态信息 state offline=>online
            user.setState("online");
            userManager_.updateState(user);

            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName();  // todo: 这些应该再客户端本地存储

            // 查询该用户是否有离线消息
            std::vector<std::string> offlineMsgList = offlineMsgManager_.query(id);
            if(!offlineMsgList.empty()) {
                response["offlinemsg"] = offlineMsgList;
                // 读取该用户的离线消息后，把该用户的所有离线消息删除掉
                // todo : mysql操作需要上锁吗
                offlineMsgManager_.remove(id);
            }

            // 查询该用户是否有离线好友申请
            std::vector<std::string> offlineNotificationList = offlineNotificationManager_.query(id);
            if(!offlineNotificationList.empty()) {
                response["offlineNotify"] = offlineNotificationList;
                // 读取该用户的离线好友申请后，不会删除掉；在得到确认时才会删除
            }
            
            // 查询该用户的好友信息并返回
            std::vector<User> friendList = friendManager_.query(id);
            if (!friendList.empty()) {
                std::vector<std::string> frinds;
                for (User &f : friendList) {
                    json friendJs;
                    friendJs["id"] = f.getId();
                    friendJs["name"] = f.getName();
                    friendJs["state"] = f.getState();
                    frinds.emplace_back(friendJs.dump());
                }
                response["friends"] = frinds;
            }

            conn->send(response.dump());
        }
    } else {
        ERROR("ID : {}, password : {} ", user.getId(), user.getPwd());
        // login failed 
        // 用户id不存在
        json response;
        if(user.getId() != id) {
            response["errno"] = 2;
            response["errmsg"] = "ID doese't exist ";
        } else {   // 密码错误
            response["errno"] = 3;
            response["errmsg"] = "Password is wrong "s;
        }
        response["msgid"] = LOGIN_MSG_ACK;
        conn->send(response.dump());
    }
}

// 处理注册业务，
void ChatService::reg(const CoTcpConnection::ptr &conn, json &js, Timestamp time) {
    std::string name = js["name"];
    std::string pwd = js["password"];
    User user;
    user.setName(name);
    user.setPwd(pwd);
    bool state = userManager_.add(user);
    if(state) {
        // register succssfully
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();
        conn->send(response.dump());
    } else {
        // regsiter failed
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
        response["name"] = name;
        conn->send(response.dump());
    }
}

void ChatService::oneChat(const CoTcpConnection::ptr &conn, json &js, Timestamp time) {
    UserId toid = js["to"].get<UserId>();
    // std::cout << "TOid: "<< toid << std::endl; // for test
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = idUserConnMap_.find(toid);
        if(it != idUserConnMap_.end()) {
            // toid在线，转发消息   服务器主动推送消息给toid用户(中转)
            // std::cout << js.dump() << std::endl; // for test
            it->second->send(std::move(js.dump()));
            return;
        }
    }
    // toid不在线，存储离线消息
    offlineMsgManager_.add(toid, js.dump());
}


void ChatService::addFriend(const CoTcpConnection::ptr &conn, json &js, Timestamp time) {
    UserId userid = js["id"].get<UserId>();  // 提出好友申请的/收到好友回复的
    UserId friendid = js["friendid"].get<UserId>();  // 收到好友申请的人/发出好友回复的
    addFriendRequestState state;
    state = addFriendRequestState(js["addFriendRequestState"].get<int>());
    js["errno"] = 0;

    UserId toid = 0;
    if(state == kApply) {  
        toid = friendid;
        // 保存消息，在返回回复时删除
        uint64_t now = Timestamp::now().microSecondsSinceEpoch();
        js["recvTime"] = now;
        offlineNotificationManager_.add(friendid, userid, now, "user", std::move(js.dump()));
    }
    else if(state == kRefuse) { 
        toid = userid;
        uint64_t recvTime = js["recvTime"];
        offlineNotificationManager_.remove(friendid, userid, recvTime, "user");
    }
    else if(state == kAgree) { // 存储好友信息
        toid = userid;
        friendManager_.addFriendship(userid, friendid);
        uint64_t recvTime = js["recvTime"];
        offlineNotificationManager_.remove(friendid, userid, recvTime, "user");
    }
    else if(state == kPending) {
        // ...
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(idUserConnMap_.find(toid) != idUserConnMap_.end()) {
            idUserConnMap_[toid]->send(std::move(js.dump()));
            return;
        }
    }

    // 提出好友申请的人不在线，存储离线消息
    if(toid == userid) { // 好友回复消息，即是普通消息
        offlineMsgManager_.add(toid, std::move(js.dump()));
    }
}

void ChatService::createGroup(const CoTcpConnection::ptr &conn, json &js, Timestamp time) {
    INFO("create group");
}

void ChatService::addGroup(const CoTcpConnection::ptr &conn, json &js, Timestamp time) {
    INFO("add group");
}

void ChatService::groupChat(const CoTcpConnection::ptr &conn, json &js, Timestamp time) {
    INFO("group chat");
}

void ChatService::logout(const CoTcpConnection::ptr &conn, json &js, Timestamp time) {
    INFO("LOGOUT");
    UserId id = js["id"].get<UserId>();
    User user = userManager_.query(id);
    assert(user.getState() == "online");

    user.setState("offline");
    userManager_.updateState(user);

    json response;
    response["msgid"] = LOGOUT_MSG;
    response["errno"] = 0;
    conn->send(response.dump());
}

void ChatService::clientCloseException(const CoTcpConnection::ptr &conn)  {
    User user;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto it = idUserConnMap_.begin(); it != idUserConnMap_.end(); ++it) {
            if (it->second == conn) {
                // 从map表删除用户的链接信息
                user.setId(it->first);
                idUserConnMap_.erase(it);
                break;
            }
        }
    }
    // 更新用户的状态信息
    if (user.getId() != -1) {
        user.setState("offline");
        userManager_.updateState(user);
    }
    
}

// 服务器异常，业务重置方法
void ChatService::reset() {
    // 把online状态的用户，设置成offline
    userManager_.resetState();
}

// 获取消息对应的处理器
MsgHandler ChatService::getHandler(int msgid) {
    // 记录错误日志，msgid没有对应的事件处理回调
    auto it = idMsgHandlerMap_.find(msgid);
    if (it == idMsgHandlerMap_.end()) {  // not find
        // 返回一个默认的处理器，空操作 --> 以免直接报错NULL操作 非法退出
        return [=](const CoTcpConnection::ptr &conn, json &js, Timestamp) {
            ERROR("msgid : {} can't find handler", msgid);
        };
    } else {
        return idMsgHandlerMap_[msgid];
    }
}

// 注册消息以及对应的Handler回调操作
ChatService::ChatService() {
    // idMsgHandlerMap_[LOGIN_MSG] = std::bind(&ChatService::login, this, _1, _2, _3);
    // 用户基本业务管理相关事件处理回调注册
    idMsgHandlerMap_.insert({LOGIN_MSG, std::bind(&ChatService::login, this, _1, _2, _3)});
    idMsgHandlerMap_.insert({LOGOUT_MSG, std::bind(&ChatService::logout, this, _1, _2, _3)});
    idMsgHandlerMap_.insert({REG_MSG, std::bind(&ChatService::reg, this, _1, _2, _3)});
    idMsgHandlerMap_.insert({ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this, _1, _2, _3)});
    idMsgHandlerMap_.insert({ADD_FRIEND_MSG, std::bind(&ChatService::addFriend, this, _1, _2, _3)});

    // 群组业务管理相关事件处理回调注册
    idMsgHandlerMap_.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3)});
    idMsgHandlerMap_.insert({ADD_GROUP_MSG, std::bind(&ChatService::addGroup, this, _1, _2, _3)});
    idMsgHandlerMap_.insert({GROUP_CHAT_MSG, std::bind(&ChatService::groupChat, this, _1, _2, _3)});
    
}
