#ifndef FRIENDMANAGER_H
#define FRIENDMANAGER_H

#include "user.h"
#include <map>
#include <string>
#include <memory>
#include <burger/db/DB.h>

using namespace burger;
using namespace burger::db;

class FriendManager {
public:
    FriendManager(std::string host = "127.0.0.1", std::string user = "root",
                    std::string passwd = "PWD", std::string dbname = "burgerChat");
    ~FriendManager() = default;

    bool addFriendship(userID smaller_id, userID greater_id);
    bool addFriendship(const User& user_1, const User& user_2);

    bool releaseFriendship(userID smaller_id, userID greater_id);
    bool releaseFriendship(const User& user_1, const User& user_2);

    bool isFriend(userID smaller_id, userID greater_id);

private:
    std::map<std::string, std::string> params_;
};


#endif // FRIENDMANAGER_H