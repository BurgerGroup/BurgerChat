#include "cmdLine.h"

void CmdHandler::help(int fd, std::string str) {
    std::cout << "show command list >>> " << std::endl;
    for (auto &p : commandMap) {
        std::cout << p.first << " : " << p.second << std::endl;
    }
    std::cout << std::endl;
}

// void CmdHandler::chat(int, std::string) {
//     int idx = str.find(":"); // friendid:message
//     if (-1 == idx) {
//         std::cerr << "chat command invalid!" << std::endl;
//         return;
//     }

//     int friendid = atoi(str.substr(0, idx).c_str());  // boost::lexical_cast
//     std::string message = str.substr(idx + 1, str.size() - idx);

//     json js;
//     js["msgid"] = ONE_CHAT_MSG;

//     js["id"] = g_currentUser.getId();
//     js["name"] = g_currentUser.getName();
//     js["toid"] = friendid;
//     js["msg"] = message;
//     js["time"] = getCurrentTime();
//     string buffer = js.dump();

//     int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
//     if (-1 == len)
//     {
//         cerr << "send chat msg error -> " << buffer << endl;
//     }
// }

