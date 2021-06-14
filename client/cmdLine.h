#ifndef CMDLINE_H
#define CMDLINE_H

#include <unordered_map>
#include <string>
#include <functional>
#include "json/json.hpp"
#include "msg.h"
#include "addFriend.h"
using json = nlohmann::json;

class ChatClient;
class CmdHandler {
public:
    static void help(ChatClient* = nullptr, const std::string& flag = "-a", const std::string& str = "");
    static void chat(ChatClient*, const std::string& flag = "-u", const std::string& = "");
    static void addFriend(ChatClient*, const std::string& flag = "", const std::string& = "");
    static void confirmFriendRequest(ChatClient*, const std::string& flag = "", const std::string& = "");
    // static void creategroup(int, std::string);
    // static void addgroup(int, std::string);
    // static void groupchat(int, std::string);
    static void logout(ChatClient*, const std::string& flag = "", const std::string& = "");
    static void clear(ChatClient*, const std::string& flag = "", const std::string& = "");
    static void showFriendList(ChatClient*, const std::string& flag = "", const std::string& = "");
    static std::unordered_map<std::string, std::string> commandMap;
    static std::unordered_map<std::string, std::function<void(ChatClient* , const std::string& , const std::string&)> > commandHandlerMap;
private: 
    static void showAllFriendList(ChatClient*);
};

#endif // CMDLINE_H