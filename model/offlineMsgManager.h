#ifndef OFFLINEMSGMANAGER_H
#define OFFLINEMSGMANAGER_H

#include <string>
#include <vector>

class OfflineMsgManager {
public:
    void add(int userid, std::string msg); // 存储用户的离线消息
    void delete(int userid); // 删除用户的离线消息
    vector<string> query(int userid); // 查询用户的离线消息
};


#endif // OFFLINEMSGMANAGER_H