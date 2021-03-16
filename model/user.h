#ifndef USER_H
#define USER_H

#include <string>

// User表的ORM类
class User {
public:
    User(int id = -1, std::string name = "", std::string pwd = "", std::string state = "offline")
    : id_(id),
    name_(name),
    password_(pwd),
    state_(state) {
    }

    void setId(int id) { id_ = id; }
    void setName(std::string name) { name_ = name; }
    void setPwd(std::string pwd) { password_ = pwd; }
    void setState(std::string state) { state_ = state; }

    int getId() const { return id_; }
    std::string getName() const { return name_; }
    std::string getPwd() const { return password_; }
    std::string getState() { return state_; }

protected:
    int id_;
    std::string name_;
    std::string password_;
    std::string state_;
};


#endif // USER_H