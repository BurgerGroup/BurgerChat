#ifndef OFFLINEMSGMANAGER_H
#define OFFLINEMSGMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <burger/db/DB.h>

using namespace burger;
using namespace burger::db;

using UserId = int64_t;


class OfflineMsgManager {
public:
    OfflineMsgManager(std::string host = "127.0.0.1", std::string user = "root",
                    std::string passwd = "PWD", std::string dbname = "burgerChat");
    ~OfflineMsgManager() = default;
    void add(UserId userid, std::string msg); // 存储用户的离线消息
    void remove(UserId userid); // 删除用户的离线消息
    std::vector<std::string> query(UserId userid); // 查询用户的离线消息
private:
    std::map<std::string, std::string> params_;
};


#endif // OFFLINEMSGMANAGER_H