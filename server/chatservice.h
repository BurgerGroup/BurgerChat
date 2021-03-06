#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <burger/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include "json/json.hpp"
#include "msg.h"

using namespace burger;
using namespace burger::net;
using json = nlohmann::json;

// 表示处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr&, json&, Timestamp)>;

// chat 业务层，singleton
class ChatService {
public:
    static ChatService& getInstance();
    // 业务接口
    // 处理登录业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注册业务
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);
private:
    ChatService();
    ~ChatService() = default;
    ChatService(const ChatService&) = delete;
    ChatService& operator=(const ChatService&) = delete; 
private:


    // 存储消息id和其对应的业务处理方法
    // std::unordered_map<MsgType, MsgHandler, std::hash<int> > idMsgHandlerMap_;
    std::unordered_map<int, MsgHandler> idMsgHandlerMap_;
};



#endif // CHATSERVICE_H