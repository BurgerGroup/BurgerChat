#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"
#include <map>
#include <string>
#include <memory>
#include <burger/db/DB.h>
#include <burger/base/Config.h>

using namespace burger;
using namespace burger::db;

// User表的数据操作类
class UserManager {
public:
    UserManager();
    ~UserManager() = default;
    bool add(User &user);
    User query(UserId id);
    bool updateState(User& user);
    void resetState();
private:
    std::map<std::string, std::string> params_;
};

#endif // USERMANAGER_H