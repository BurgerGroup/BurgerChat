#include "groupManager.h"
using namespace burger::db;
GroupManager::GroupManager(std::string host, std::string user,
                    std::string passwd, std::string dbname) 
{
    params_["host"] = host;
    params_["user"] = user;
    params_["passwd"] = passwd;
    params_["dbname"] = dbname;
}

bool GroupManager::add(Group &group) 
{
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql = "insert into AllGroup (groupName, groupDesc) values(?,?)";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    stmt->bind(1, group.getName());
    stmt->bind(2, group.getDesc());
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    group.setId(mysql->getLastInsertId());
    return true;
}

bool GroupManager::dismiss(groupID id) 
{
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql = "delete from Friend where userid = ?";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    stmt->bind(1, id);
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    return true;
}

Group GroupManager::query(groupID id) 
{
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql = "select * from AllGroup where id = ?";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return Group();
    }
    stmt->bind(1, id);
    auto stmtRes = stmt->query();
    while(stmtRes->next()) {
        Group group;
        group.setId(stmtRes->getInt64(0));
        group.setName(stmtRes->getString(1));
        group.setDesc(stmtRes->getString(2));
        return group;
    }
    return Group();
}

bool GroupManager::modify(groupID id) 
{
    // ...what?
    return true;
}
