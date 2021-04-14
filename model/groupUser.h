#ifndef GROUPUSER_H
#define GROUPUSER_H


#include "user.h"
#include "group.h"
#include <string>
 
typedef std::pair<groupID, UserId> groupUserID;

class GroupUser {
public:
    GroupUser(groupID groupid = -1, UserId userid = -1, const std::string& role = "normal")
        : groupid_(groupid),
          userid_(userid),
          role_(role)
    {}

    ~GroupUser() = default;

    void setGroupId(groupID id) { groupid_ = id; }
    void setUserId(UserId id) { userid_ = id; }
    void setRole(const std::string& role) { role_ = role; }

    groupID getGroupId() const {return  groupid_; }
    UserId getUserId() const { return userid_; }
    std::string getRole() const { return role_ ; }

private:
    groupID groupid_;
    UserId userid_;
    std::string role_;
};


#endif // GROUPUSER_H