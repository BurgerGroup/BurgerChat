#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"
#include <map>
#include <string>
#include <memory>
#include <burger/db/DB.h>

using namespace burger;
using namespace burger::db;

// User表的数据操作类
class UserManager {
public:
    UserManager(std::string host = "127.0.0.1", std::string user = "root",
                    std::string passwd = "PWD", std::string dbname = "burgerChat");
    ~UserManager() = default;
    bool add(User &user);
    User query(int64_t id);
    bool updateState(User& user);
    void resetState();
private:
    std::map<std::string, std::string> params_;
};


#endif // USERMANAGER_H