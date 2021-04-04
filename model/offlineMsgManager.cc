#include "offlineMsgManager.h"


OfflineMsgManager::OfflineMsgManager(std::string host, std::string user,
                    std::string passwd, std::string dbname) {
    params_["host"] = host;
    params_["user"] = user;
    params_["passwd"] = passwd;
    params_["dbname"] = dbname;
}

void OfflineMsgManager::add(UserId userid, std::string msg) {
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql =  "insert into OfflineMsg (userid, message) values(?, ?)";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return;
    }
    stmt->bind(1, userid);
    stmt->bind(2, msg);
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return;
    }
}

void OfflineMsgManager::remove(UserId userid) {
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql =  "delete from OfflineMsg where userid=?";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return ;
    }
    stmt->bind(1, userid);
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return;
    } 
}

// todo: efficient?
std::vector<std::string> OfflineMsgManager::query(UserId userid) {
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::vector<std::string> vec;
    std::string sql = "select message from OfflineMsg where userid = ?";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return vec;
    }

    stmt->bind(1, userid);
    auto stmtRes = stmt->query();
    while(stmtRes->next()) {
        vec.emplace_back(stmtRes->getString(0));
    }
    return vec;
}


