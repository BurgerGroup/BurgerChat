#include "offlineNotificationManager.h"


OfflineNotificationManager::OfflineNotificationManager(std::string host, std::string user,
                    std::string passwd, std::string dbname) {
    params_["host"] = host;
    params_["user"] = user;
    params_["passwd"] = passwd;
    params_["dbname"] = dbname;
}

void OfflineNotificationManager::add(UserId userid, UserId fromid, uint64_t recvTime, 
            const std::string& fromType, const std::string& msg) {
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql =  "insert into OfflineNotification (userid, fromid, recvTime, fromType, message) values(?, ?, ?, ?, ?)";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return;
    }
    stmt->bind(1, userid);
    stmt->bind(2, fromid);
    stmt->bind(3, recvTime);
    stmt->bind(4, fromType);
    stmt->bind(5, msg);
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return;
    }
}

void OfflineNotificationManager::remove(UserId userid, UserId fromid, uint64_t recvTime, 
            const std::string& fromType) {
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql =  "delete from OfflineNotification where userid=? and fromid=? and recvTime = ? and fromType = ?";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return ;
    }
    stmt->bind(1, userid);
    stmt->bind(2, fromid);
    stmt->bind(3, recvTime);
    stmt->bind(4, fromType);
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return;
    } 
}

// todo: efficient?
// std::vector<std::string> OfflineNotificationManager::query(UserId userid, UserId fromid, const std::string& fromType = "user") {
//     MySQL::ptr mysql = std::make_shared<MySQL>(params_);
//     mysql->connect();
//     std::vector<std::string> vec;
//     std::string sql = "select message from OfflineNotification where userid=? and fromid=? and fromType = ?";
//     auto stmt = mysql->prepare(sql);
//     if(!stmt) {
//         ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
//         return vec;
//     }
//     stmt->bind(1, userid);
//     stmt->bind(2, fromid);
//     stmt->bind(3, fromType);
//     auto stmtRes = stmt->query();
//     while(stmtRes->next()) {
//         vec.emplace_back(stmtRes->getString(0));
//     }
//     return vec;
// }

std::vector<std::string> OfflineNotificationManager::query(UserId userid) {
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::vector<std::string> vec;
    std::string sql = "select message from OfflineNotification where userid=?";
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