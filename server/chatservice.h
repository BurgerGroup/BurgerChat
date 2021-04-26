#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <burger/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <vector>
#include "json/json.hpp"
#include "msg.h"
#include "addFriend.h"
#include "userManager.h"
#include "OfflineMsgManager.h"
#include "offlineNotificationManager.h"
#include "friendManager.h"

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
    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 添加好友业务
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 加入群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注销业务
    void logout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);
    // 服务器异常，业务重置方法
    void reset();
    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);
private:
    ChatService();
    ~ChatService() = default;
    ChatService(const ChatService&) = delete;
    ChatService& operator=(const ChatService&) = delete; 
private:

    UserManager userManager_;
    OfflineMsgManager offlineMsgManager_;
    OfflineNotificationManager offlineNotificationManager_;
    FriendManager friendManager_;
    // 存储消息id和其对应的业务处理方法
    // std::unordered_map<MsgType, MsgHandler, std::hash<int> > idMsgHandlerMap_;
    std::unordered_map<int, MsgHandler> idMsgHandlerMap_;
    // 在线用户的通信连接，我们需要长连接
    std::unordered_map<UserId, TcpConnectionPtr> idUserConnMap_;
    std::mutex mutex_;
};



#endif // CHATSERVICE_H