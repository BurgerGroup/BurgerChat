#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H

#include "group.h"
#include <map>
#include <string>
#include <memory>
#include <burger/db/DB.h>
#include <burger/base/Config.h>

using namespace burger::db;
using namespace burger;

// 对群组表的操作
class GroupManager {
public:
    GroupManager();
    ~GroupManager()= default;

    bool add(Group &group);
    bool dismiss(groupID id);
    Group query(groupID id);
    bool modify(groupID id);
    
private:
    std::map<std::string, std::string> params_;
};


#endif // GROUPMANAGER_H