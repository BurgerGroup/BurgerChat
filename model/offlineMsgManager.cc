#include "offlineMsgManager.h"

OfflineMsgManager::OfflineMsgManager() {
    params_["host"] = Config::Instance().getString("mysql", "host", "127.0.0.1");
    params_["user"] = Config::Instance().getString("mysql", "user", "root");
    params_["passwd"] = Config::Instance().getString("mysql", "password", "PWD");
    params_["dbname"] = Config::Instance().getString("mysql", "dbname", "burgerChat");
}

bool OfflineMsgManager::add(UserId userid, std::string msg) {
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql =  "insert into OfflineMsg (userid, message) values(?, ?)";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    stmt->bind(1, userid);
    stmt->bind(2, msg);
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    return true;
}

bool OfflineMsgManager::remove(UserId userid) {
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql =  "delete from OfflineMsg where userid=?";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    stmt->bind(1, userid);
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    } 
    return true;
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


