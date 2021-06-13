#include "groupManager.h"

GroupManager::GroupManager() {
    params_["host"] = Config::Instance().getString("mysql", "host", "127.0.0.1");
    params_["user"] = Config::Instance().getString("mysql", "user", "root");
    params_["passwd"] = Config::Instance().getString("mysql", "password", "PWD");
    params_["dbname"] = Config::Instance().getString("mysql", "dbname", "burgerChat");
}

bool GroupManager::add(Group &group) {
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
