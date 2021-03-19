#include "friendManager.h"
using namespace burger;
using namespace burger::db;

FriendManager::FriendManager(std::string host, std::string user,
                    std::string passwd, std::string dbname) 
{
    params_["host"] = host;
    params_["user"] = user;
    params_["passwd"] = passwd;
    params_["dbname"] = dbname;
}

bool FriendManager::addFriendship(int64_t smaller_id, int64_t greater_id) 
{
    if(smaller_id == greater_id) return false;
    if(smaller_id > greater_id)
    {
        std::swap(smaller_id, greater_id);
    }

    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql = "insert into Friend (user1_Id, user2_Id) values(?,?)";
    stmt->bind(1, smaller_id);
    stmt->bind(2, greater_id);
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    return true;
}

bool FriendManager::addFriendship(const User& user_1, const User& user_2) 
{
    return addFriendship(user_1.getId(), user_2.getId());
}

bool FriendManager::releaseFriendship(int64_t smaller_id, int64_t greater_id) 
{
    if(smaller_id == greater_id) return false;
    if(smaller_id > greater_id)
    {
        std::swap(smaller_id, greater_id);
    }

    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql = "delete from Friend where user1_Id = ? and user2_Id = ?";
    stmt->bind(1, smaller_id);
    stmt->bind(2, greater_id);
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    return true;
}

bool FriendManager::releaseFriendship(const User& user_1, const User& user_2) 
{
    return releaseFriendship(user_1.getId(), user_2.getId());
}
