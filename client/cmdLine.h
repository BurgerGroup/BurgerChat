#ifndef CMDLINE_H
#define CMDLINE_H

#include <unordered_map>
#include <string>
#include <functional>
#include "json/json.hpp"
#include "msg.h"
using json = nlohmann::json;

class CmdHandler {
public:
    static void help(int fd = 0, std::string str = "");
    // static void chat(int, std::string);
    // static void addfriend(int, std::string);
    // static void creategroup(int, std::string);
    // static void addgroup(int, std::string);
    // static void groupchat(int, std::string);
    // static void loginout(int, std::string);
};


std::unordered_map<std::string, std::string> commandMap = {
    {"help", "显示所有支持的命令，格式help"},
    {"chat", "一对一聊天，格式chat:friendid:message"},
    {"addfriend", "添加好友，格式addfriend:friendid"},
    {"creategroup", "创建群组，格式creategroup:groupname:groupdesc"},
    {"addgroup", "加入群组，格式addgroup:groupid"},
    {"groupchat", "群聊，格式groupchat:groupid:message"},
    {"loginout", "注销，格式loginout"}
};

// 注册系统支持的客户端命令处理
std::unordered_map<std::string, std::function<void(int, std::string)>> commandHandlerMap = {
    {"help", CmdHandler::help},
    // {"chat", CmdHandler::chat},
    // {"addfriend", CmdHandler::addfriend},
    // {"creategroup", CmdHandler::creategroup},
    // {"addgroup", CmdHandler::addgroup},
    // {"groupchat", CmdHandler::groupchat},
    // {"loginout", CmdHandler::loginout}
};

#endif // CMDLINE_H