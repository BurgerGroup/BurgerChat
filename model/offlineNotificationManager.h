#ifndef OFFLINENOTIFICATIONMANAGER_H
#define OFFLINENOTIFICATIONMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <burger/base/Timestamp.h>
#include <burger/db/DB.h>

using namespace burger;
using namespace burger::db;

using UserId = int64_t;


class OfflineNotificationManager {
public:
    OfflineNotificationManager(std::string host = "127.0.0.1", std::string user = "root",
                    std::string passwd = "PWD", std::string dbname = "burgerChat");
    ~OfflineNotificationManager() = default;
    void add(UserId userid, UserId fromid, uint64_t recvTime, 
            const std::string& fromType = "user", const std::string& msg = ""); // 存储用户的离线好友通知

    void remove(UserId userid, UserId fromid, uint64_t recvTime, 
            const std::string& fromType = "user"); // 删除用户的离线通知
            
    // std::vector<std::string> query(UserId userid, UserId fromid, const std::string& fromType = "user"); // 查询用户的离线通知
    std::vector<std::string> query(UserId userid);
private:
    std::map<std::string, std::string> params_;
};


#endif // OFFLINENOTIFICATIONMANAGER_H