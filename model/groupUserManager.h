#ifndef GROUPUSERMANAGER_H
#define GROUPUSERMANAGER_H


#include "groupUser.h"
#include <map>
#include <string>
#include <memory>
#include <burger/db/DB.h>
#include <burger/base/Config.h>

using namespace burger;
using namespace burger::db;

// 对群组表的操作
class GroupUserManager
{
public:
    GroupUserManager();
    ~GroupUserManager()= default;

    bool add(GroupUser& groupUser);
    bool remove(groupUserID id);
    GroupUser query(groupUserID id);
    bool modify(groupUserID id);
    
private:
    std::map<std::string, std::string> params_;
};


#endif // GROUPUSERMANAGER_H