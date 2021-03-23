#ifndef __GROUP_H__
#define __GROUP_H__
#include <string>
#include <list>

typedef int64_t groupID;
class Group
{
public:
    Group(std::string name = "", std::string desc = "A New Group.", groupID id = -1)
        : name_(name),
          desc_(desc),
          id_(id)    
    {}
    ~Group() = default;

    void setId(groupID id) { id_ = id; }
    void setName(std::string name) { name_ = name; }
    void setDesc(std::string desc) { desc_ = desc; }

    groupID getId() const { return id_; }
    std::string getName() const { return name_; }
    std::string getDesc() const { return desc_; }
    
    // 添加群成员
    // void addMember(const User& user) {}
private:
    std::string name_;
    std::string desc_;
    groupID id_;
    // userID groupLeader_; // 群主
    // std::list<userID> members_; // 群成员
};

#endif // __GROUP_H__