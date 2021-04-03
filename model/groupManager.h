#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H

#include "group.h"
#include <map>
#include <string>
#include <memory>
#include <burger/db/DB.h>

// 对群组表的操作
class GroupManager {
public:
    GroupManager(std::string host = "127.0.0.1", std::string user = "root",
                    std::string passwd = "PWD", std::string dbname = "burgerChat");
    ~GroupManager()= default;

    bool add(Group &group);
    bool dismiss(groupID id);
    Group query(groupID id);
    bool modify(groupID id);
    
private:
    std::map<std::string, std::string> params_;
};


#endif // GROUPMANAGER_H