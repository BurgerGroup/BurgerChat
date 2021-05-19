#include "cmdLine.h"
#include "chatClient.h"
#include "color.h"

std::unordered_map<std::string, std::string> CmdHandler::commandMap = {
    {"help", "显示所有支持的命令，格式::help"},
    {"chat", "一对一聊天，格式:chat:friendid:message"},
    {"addFriend", "添加好友，格式:addFriend:friendid"},
    {"confirmFriendRequest", "查看好友请求，格式:confirmFriendRequest"},
    // {"creategroup", "创建群组，格式:creategroup:groupname:groupdesc"},
    // {"addgroup", "加入群组，格式:addgroup:groupid"},
    // {"groupchat", "群聊，格式:groupchat:groupid:message"},
    {"logout", "注销，格式:logout"}
};

// 注册系统支持的客户端命令处理
std::unordered_map<std::string, std::function<void(ChatClient* , const std::string& )>> CmdHandler::commandHandlerMap = {
    {"help", CmdHandler::help},
    {"chat", CmdHandler::chat},
    {"addFriend", CmdHandler::addFriend},
    {"confirmFriendRequest", CmdHandler::confirmFriendRequest},
    // {"creategroup", CmdHandler::creategroup},
    // {"addgroup", CmdHandler::addgroup},
    // {"groupchat", CmdHandler::groupchat},
    {"logout", CmdHandler::logout}
};

void CmdHandler::help(ChatClient* client, const std::string& str) {
    client->outputMsg(">> show command list: ");
    std::cout << "1111" << std::endl;
    for (auto &p : commandMap) {
        std::string msg = p.first + ":" + p.second;
        client->outputMsg(msg, true);
    }
}

void CmdHandler::chat(ChatClient* client, const std::string& msg) {
    size_t idx = msg.find(":"); // friendid:message
    if (idx == msg.npos) {
        std::cout << RED << "chat command invalid!" << std::endl;
        return;
    }

    UserId friendid = atoi(msg.substr(0, idx).c_str());  // boost::lexical_cast

    if(!client->info_->hasFriend(friendid)) {
        std::cout << RED << "This User is not your friend yet!!" << std::endl;
        return;
    }

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

void CmdHandler::addFriend(ChatClient* client, const std::string& msg) {
    UserId friendid = atoi(msg.c_str()); 

    // 如果已经是好友，直接返回提示消息即可
    if(client->info_->hasFriend(friendid)) {
        std::cout << RED << "User " + std::to_string(friendid) + " is your friend already!" << std::endl;
        std::cout << RESET;
        return;
    }   

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

void CmdHandler::confirmFriendRequest(ChatClient* client, const std::string&) {
    if(client->friendRequests_.empty()) {
        std::cout << YELLOW << "You don't have any new friend requests!!!" << std::endl;
        return;
    }

    std::cout << YELLOW << "You have " << client->friendRequests_.size() <<" new friend requests in total!" << std::endl;
    auto request = std::move(client->friendRequests_.front());
    client->friendRequests_.pop();

    UserId userid = request["id"].get<UserId>();
    std::string name = request["name"];

    json response(request);
    bool hasChoosed = false;
    while(!hasChoosed) {
        std::cout << YELLOW << userid << " (" << name << ")" << " wants to MAKE FRIEND with you!!!" << std::endl;
        std::cout << ">>> Input : 1.Agree  2.Refuse  3.Handle it later... <<< " << std::endl;
        std::cout << RESET;

        std::string input;
        std::getline(std::cin, input);
        int choice = atoi(input.c_str());
        switch (choice) {
            case 1: {
                response["addFriendRequestState"] = kAgree;
                User newFriend(name, "", "online", userid);
                client->info_->setFriends(std::move(newFriend));
                hasChoosed = true;
                break;
            }
            case 2: {
                response["addFriendRequestState"] = kRefuse;
                hasChoosed = true;
                break;
            }
            case 3: {
                response["addFriendRequestState"] = kPending;
                hasChoosed = true;
                break;
            } 
            default: {
                response["addFriendRequestState"] = kPending;
                hasChoosed = false;
                break;
            }
        }
    }
    std::string content = response.dump();
    client->send(std::move(content));
}