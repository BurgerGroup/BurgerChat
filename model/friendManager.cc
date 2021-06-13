#include "friendManager.h"

FriendManager::FriendManager() {
    params_["host"] = Config::Instance().getString("mysql", "host", "127.0.0.1");
    params_["user"] = Config::Instance().getString("mysql", "user", "root");
    params_["passwd"] = Config::Instance().getString("mysql", "password", "PWD");
    params_["dbname"] = Config::Instance().getString("mysql", "dbname", "burgerChat");
}

bool FriendManager::addFriendship(UserId smaller_id, UserId greater_id) {
    if(smaller_id == greater_id) return false;
    if(isFriend(smaller_id, greater_id)) return false;
    if(smaller_id > greater_id) {
        std::swap(smaller_id, greater_id);
    }

    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    
    std::string sql = "insert into Friend (userid, friendid) values(?,?)";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    stmt->bind(1, smaller_id);
    stmt->bind(2, greater_id);
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    return true;
}

bool FriendManager::addFriendship(const User& user_1, const User& user_2) {
    return addFriendship(user_1.getId(), user_2.getId());
}

bool FriendManager::releaseFriendship(UserId smaller_id, UserId greater_id) {
    if(smaller_id == greater_id) return false;
    if(smaller_id > greater_id) {
        std::swap(smaller_id, greater_id);
    }

    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql = "delete from Friend where userid = ? and friendid = ?";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    stmt->bind(1, smaller_id);
    stmt->bind(2, greater_id);
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    return true;
}

bool FriendManager::releaseFriendship(const User& user_1, const User& user_2) {
    return releaseFriendship(user_1.getId(), user_2.getId());
}

bool FriendManager::isFriend(UserId smaller_id, UserId greater_id) {
    if(smaller_id == greater_id) return true; // 自己是自己的好友
    if(smaller_id > greater_id) {
        std::swap(smaller_id, greater_id);
    }

    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql = "select * from Friend where userid = ? and friendid = ?";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    stmt->bind(1, smaller_id);
    stmt->bind(2, greater_id);
    auto stmtRes = stmt->query();
    while(stmtRes->next()) {
        return true;
    }
    return false;
}

// 返回用户好友列表
std::vector<User> FriendManager::query(UserId userid) {
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::vector<User> vec;

    std::string sql = "select a.id,a.name,a.state from User a inner join Friend b on b.friendid = a.id where b.userid=?";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return vec;
    }

    stmt->bind(1, userid);
    auto stmtRes = stmt->query();
    while(stmtRes->next()) {
        User user;
        user.setId(stmtRes->getInt64(0));
        user.setName(stmtRes->getString(1));
        user.setState(stmtRes->getString(2));
        vec.emplace_back(user);
    }

    sql = "select a.id,a.name,a.state from User a inner join Friend b on b.userid = a.id where b.friendid=?";
    stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return vec;
    }

    stmt->bind(1, userid);
    stmtRes = stmt->query();
    while(stmtRes->next()) {
        User user;
        user.setId(stmtRes->getInt64(0));
        user.setName(stmtRes->getString(1));
        user.setState(stmtRes->getString(2));
        vec.emplace_back(user);
    }
    return vec;
}
