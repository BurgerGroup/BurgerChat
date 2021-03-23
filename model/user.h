#ifndef USER_H
#define USER_H

#include <string>

typedef int64_t userID;
// User表的ORM类
class User {
public:
    User(std::string name = "", std::string pwd = "", std::string state = "offline", userID id = -1)
    : name_(name),
    password_(pwd),
    state_(state),
    id_(id){
    }
    ~User() = default;
    void setId(userID id) { id_ = id; }
    void setName(std::string name) { name_ = name; }
    void setPwd(std::string pwd) { password_ = pwd; }
    void setState(std::string state) { state_ = state; }

    userID getId() const { return id_; }
    std::string getName() const { return name_; }
    std::string getPwd() const { return password_; }
    std::string getState() { return state_; }

protected:
    std::string name_;
    std::string password_;
    std::string state_;
    userID id_;
};


#endif // USER_H