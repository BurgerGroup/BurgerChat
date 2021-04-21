#include "chatservice.h"
#include <burger/base/Log.h>

using namespace std::placeholders;

ChatService& ChatService::getInstance() {
    static ChatService service;
    return service;
}

// id pwd pwd
void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    UserId id = js["id"].get<UserId>();
    std::string pwd = js["password"];
    User user = userManager_.query(id);

    if(user.getId() == id && user.getPwd() == pwd) {
        if(user.getState() == "online") {
            // 该用户已经登录，不允许重复登录 
            // todo 把挤下线的功能
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["errmsg"] = "this account is using, input another!";
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
            std::vector<std::string> offlineList = offlineManager_.query(id);
            if(!offlineList.empty()) {
                response["offlinemsg"] = offlineList;
                // 读取该用户的离线消息后，把该用户的所有离线消息删除掉
                offlineManager_.remove(id);
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
        std::cout << user.getId() <<  " " << user.getPwd() << std::endl; // for test
        // login failed , 该用户不存在，用户存在但是密码错误
        // todo : 这里可以做的更细致
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "id or password is invalid!";
        conn->send(response.dump());
    }
}

// 处理注册业务，
void ChatService::reg(const TcpConnectionPtr &conn, json &js, Timestamp time) {
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

void ChatService::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time) {
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
    offlineManager_.add(toid, js.dump());
}


void ChatService::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    UserId userid = js["id"].get<UserId>();
    UserId friendid = js["friendid"].get<UserId>();
    addFriendRequestState state;
    state = addFriendRequestState(js["addFriendRequestState"].get<int>());
    js["errno"] = 0;

    // todo: 如果已经是好友，直接返回提示消息即可

    if(state == kPending) {
        // todo : 再创建一个表？还是直接存放在离线消息表里面，每次上线都提醒。
    }

    UserId toid = 0;
    if(state == kApply) {
        toid = friendid;
    }
    else if(state == kRefuse) {
        toid = userid;
    }
    else if(state == kAgree) { // 存储好友信息
        toid = userid;
        friendManager_.addFriendship(userid, friendid);
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(idUserConnMap_.find(toid) != idUserConnMap_.end()) {
            idUserConnMap_[toid]->send(std::move(js.dump()));
            return;
        }
    }
    // toid不在线，存储离线消息
    offlineManager_.add(toid, std::move(js.dump()));
}

void ChatService::createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    INFO("create group");
}

void ChatService::addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    INFO("add group");
}

void ChatService::groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    INFO("group chat");
}

void ChatService::logout(const TcpConnectionPtr &conn, json &js, Timestamp time) {
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

void ChatService::clientCloseException(const TcpConnectionPtr &conn)  {
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
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp) {
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
