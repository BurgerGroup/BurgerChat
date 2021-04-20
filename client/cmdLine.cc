#include "cmdLine.h"
#include "chatClient.h"
#include "color.h"

std::unordered_map<std::string, std::string> CmdHandler::commandMap = {
    {"help", "显示所有支持的命令，格式help"},
    {"chat", "一对一聊天，格式chat:friendid:message"},
    {"addfriend", "添加好友，格式addfriend:friendid"},
    {"creategroup", "创建群组，格式creategroup:groupname:groupdesc"},
    {"addgroup", "加入群组，格式addgroup:groupid"},
    {"groupchat", "群聊，格式groupchat:groupid:message"},
    {"logout", "注销，格式logout"}
};

// 注册系统支持的客户端命令处理
std::unordered_map<std::string, std::function<void(ChatClient* , const std::string& )>> CmdHandler::commandHandlerMap = {
    {"help", CmdHandler::help},
    {"chat", CmdHandler::chat},
    // {"addfriend", CmdHandler::addfriend},
    // {"creategroup", CmdHandler::creategroup},
    // {"addgroup", CmdHandler::addgroup},
    // {"groupchat", CmdHandler::groupchat},
    {"logout", CmdHandler::logout}
};

void CmdHandler::help(ChatClient*, const std::string& str) {
    std::cout << GREEN << "show command list >>> " << std::endl;
    for (auto &p : commandMap) {
        std::cout << GREEN << p.first << " : " << p.second << std::endl;
    }
    std::cout << std::endl;
}

void CmdHandler::chat(ChatClient* client, const std::string& msg) {
    size_t idx = msg.find(":"); // friendid:message
    if (idx == msg.npos) {
        std::cout << RED << "chat command invalid!" << std::endl;
        return;
    }

    UserId friendid = atoi(msg.substr(0, idx).c_str());  // boost::lexical_cast
    std::string message = msg.substr(idx + 1, msg.size() - idx);

    json js;
    js["msgid"] = ONE_CHAT_MSG;

    js["id"] = client->getInfo()->getId();
    js["from"] = client->getInfo()->getName();
    js["to"] = friendid;
    js["msg"] = message;

    // TODO: add time
    // js["time"] = getCurrentTime();
    std::string content = js.dump();
    // std::cout << content << std::endl; // for test
    client->send(std::move(content));
}

void CmdHandler::logout(ChatClient* client, const std::string& msg) {
    assert(client->getLogInState() == ChatClient::kLoggedIn);
    client->setLogInState_(ChatClient::kLogging);

    json js;
    js["msgid"] = LOGOUT_MSG;
    js["id"] = client->getInfo()->getId();
    // js["name"] = client->getInfo()->getName();

    std::string content = js.dump();
    // std::cout << content << std::endl; // for test
    client->send(std::move(content));
}

void CmdHandler::addfriend(ChatClient* client, const std::string& msg) {
    UserId friendid = atoi(msg.c_str()); 
    json js;
    js["msgid"] = ADD_FRIEND_MSG;

    js["id"] = client->getInfo()->getId();
    js["name"] = client->getInfo()->getName();
    js["friendid"] = friendid;
    js["addFriendRequestState"] = kApply;

    std::string content = js.dump();
    // std::cout << content << std::endl; // for test
    client->send(std::move(content));
}