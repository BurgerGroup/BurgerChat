#ifndef INFO_H
#define INFO_H
#include "user.h"
#include "winManager.h"
#include <memory>
#include <vector>

// 记录当前用户的信息
class Info {
public:
    Info(std::string name = "", std::string pwd = "", std::string state = "offline", UserId id = -1)
        : currentUser_(name, pwd, state, id) {}
    Info(const User& user) : currentUser_(user) {}
    ~Info() = default;

    UserId getId() const { return currentUser_.getId(); }
    std::string getName() const { return currentUser_.getName(); }
    std::string getPwd() const { return currentUser_.getPwd(); }
    std::string getState() { return currentUser_.getState(); }

    void setId(UserId id) { currentUser_.setId(id); }
    void setName(const std::string& name) { currentUser_.setName(name); }
    void setPwd(const std::string& pwd) { currentUser_.setPwd(pwd); }
    void setState(const std::string& state) { currentUser_.setState(state); }

private:
    User currentUser_;     // 记录当前系统登录的用户信息
    std::vector<User> currentUserFriendList_;
    // std::vector<Group> currentUserGroupList_;     // 记录当前登录用户的群组列表信息
};

#endif // INFO_H