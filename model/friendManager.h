#ifndef __FRIENDMANAGER_H__
#define __FRIENDMANAGER_H__
#include "user.h"
#include <map>
#include <string>
#include <memory>
#include <burger/db/DB.h>

using namespace burger;
using namespace burger::db;


class FriendManager
{
public:
    FriendManager(std::string host = "127.0.0.1", std::string user = "root",
                    std::string passwd = "PWD", std::string dbname = "burgerChat");
    ~FriendManager() = default;

    bool addFriendship(int64_t smaller_id, int64_t greater_id);
    bool addFriendship(const User& user_1, const User& user_2);

    bool releaseFriendship(int64_t smaller_id, int64_t greater_id);
    bool releaseFriendship(const User& user_1, const User& user_2);

private:
    std::map<std::string, std::string> params_;
}

#endif // __FRIENDMANAGER_H__