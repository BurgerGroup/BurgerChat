#ifndef __GROUPUSERMANAGER_H__
#define __GROUPUSERMANAGER_H__
#include "groupUser.h"
#include <map>
#include <string>
#include <memory>
#include <burger/db/DB.h>

// 对群组表的操作
class GroupUserManager
{
public:
    GroupUserManager(std::string host = "127.0.0.1", std::string user = "root",
                    std::string passwd = "PWD", std::string dbname = "burgerChat");
    ~GroupUserManager()= default;

    bool add(GroupUser& groupUser);
    bool remove(groupUserID id);
    GroupUser query(groupUserID id);
    bool modify(groupUserID id);
    
private:
    std::map<std::string, std::string> params_;
};

#endif // __GROUPUSERMANAGER_H__