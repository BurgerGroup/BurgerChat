#ifndef FRIENDMANAGER_H
#define FRIENDMANAGER_H

#include "user.h"
#include <map>
#include <string>
#include <memory>
#include <burger/db/DB.h>
#include <burger/base/Config.h>

using namespace burger;
using namespace burger::db;

class FriendManager {
public:
    FriendManager();
    ~FriendManager() = default;

    bool addFriendship(UserId smaller_id, UserId greater_id);
    bool addFriendship(const User& user_1, const User& user_2);

    bool releaseFriendship(UserId smaller_id, UserId greater_id);
    bool releaseFriendship(const User& user_1, const User& user_2);

    bool isFriend(UserId smaller_id, UserId greater_id);

    // 返回用户好友列表
    // todo : 保存到本地？
    std::vector<User> query(UserId userid);

private:
    std::map<std::string, std::string> params_;
};


#endif // FRIENDMANAGER_H