#ifndef INFO_H
#define INFO_H
#include "user.h"
#include "json/json.hpp"
#include "winManager.h"
#include <memory>
#include <unordered_map>

using json = nlohmann::json;
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
    const std::unordered_map<UserId, User>& getFriendList() const { return currentUserFriendList_; } 

    void setId(UserId id) { currentUser_.setId(id); }
    void setName(const std::string& name) { currentUser_.setName(name); }
    void setPwd(const std::string& pwd) { currentUser_.setPwd(pwd); }
    void setState(const std::string& state) { currentUser_.setState(state); }
    void setFriends(const User& newFriend) { currentUserFriendList_[newFriend.getId()] = newFriend; } 
    void setFriends(const std::vector<std::string>& friends) {
        for(const auto& f : friends) {
            json newFriend = json::parse(f);
            UserId id = newFriend["id"].get<UserId>();
            User user(newFriend["name"], "", newFriend["state"], id);
            currentUserFriendList_[id] = std::move(user);
        }
    }
    bool hasFriend(UserId id) { return currentUserFriendList_.find(id) != currentUserFriendList_.end(); }

private:
    User currentUser_;     // 记录当前系统登录的用户信息
    std::unordered_map<UserId, User> currentUserFriendList_;
    // std::vector<Group> currentUserGroupList_;     // 记录当前登录用户的群组列表信息
};

#endif // INFO_H