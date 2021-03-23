#include "groupUserManager.h"
using namespace burger::db;
GroupUserManager::GroupUserManager(std::string host, std::string user,
                    std::string passwd, std::string dbname) 
{
    params_["host"] = host;
    params_["user"] = user;
    params_["passwd"] = passwd;
    params_["dbname"] = dbname;
}

bool GroupUserManager::add(GroupUser& groupUser) 
{
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql = "insert into GroupUser (groupid, userid, groupRole) values(?,?,?)";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    stmt->bind(1, groupUser.getGroupId());
    stmt->bind(2, groupUser.getUserId());
    stmt->bind(3, groupUser.getRole());
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    return true;
}

bool GroupUserManager::remove(groupUserID id) 
{
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql = "delete from GroupUser where groupid = ? and userid = ?";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    stmt->bind(1, id.first);
    stmt->bind(2, id.second);

    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    return true;
}

GroupUser GroupUserManager::query(groupUserID id) 
{
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql = "select * from GroupUser where groupid = ? and userid = ?";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return GroupUser();
    }
    stmt->bind(1, id.first);
    stmt->bind(2, id.second);

    auto stmtRes = stmt->query();
    while(stmtRes->next()) {
        GroupUser groupUser;
        groupUser.setGroupId(stmtRes->getInt64(0));
        groupUser.setUserId(stmtRes->getInt64(1));
        groupUser.setRole(stmtRes->getString(2));
        return groupUser;
    }
    return GroupUser();
}

bool GroupUserManager::modify(groupUserID id) 
{
    // what?
    return true;
}
