#include "chatservice.h"
#include <burger/base/Log.h>

using namespace std::placeholders;

ChatService& ChatService::getInstance() {
    static ChatService service;
    return service;
}

void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    INFO("ChatService::login");
}

// 处理注册业务，
void ChatService::reg(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    std::name = js["name"];
    std::string pwd = js["passwd"];
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
        conn->send(response.dump());
    }
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

    idMsgHandlerMap_.insert({REG_MSG, std::bind(&ChatService::reg, this, _1, _2, _3)});    
    
}
