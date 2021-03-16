#ifndef USERMANAGER_H
#define USERMANAGER_H


#include "user.h"
#include <map>
#include <string>
#include <burger/db/DB.h>
// User表的数据操作类
class UserManager {
public:
    UserManager(std::string host = "127.0.0.1", std::string user = "root",
                    std::string passwd = "PWD", std::string dbname = "burger");
    ~UserManager() = default;
    bool add(User &user);
    User query(int id);
    bool updateState(User user);
    void resetState();
private:
    std::map<std::string, std::string> params_;
};


#endif // USERMANAGER_H