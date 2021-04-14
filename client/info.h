#ifndef INFO_H
#define INFO_H
#include "user.h"
#include "winManager.h"
#include <memory>
#include <vector>

// 记录当前用户的信息
class Info {
public:
    Info() = default;
    ~Info() = default;

    UserId getId() const { return currentUser_.getId(); }
    std::string getName() const { return currentUser_.getName(); }
    std::string getPwd() const { return currentUser_.getPwd(); }
    std::string getState() { return currentUser_.getState(); }
private:
    User currentUser_;     // 记录当前系统登录的用户信息
    std::vector<User> currentUserFriendList_;
    // std::vector<Group> currentUserGroupList_;     // 记录当前登录用户的群组列表信息
};

#endif // INFO_H