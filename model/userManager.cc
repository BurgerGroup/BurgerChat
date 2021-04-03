#include "userManager.h"

// todo : 优化 ：抽个类出来
using namespace burger::db;
UserManager::UserManager(std::string host, std::string user,
                    std::string passwd, std::string dbname) {
    params_["host"] = host;
    params_["user"] = user;
    params_["passwd"] = passwd;
    params_["dbname"] = dbname;
}

bool UserManager::add(User &user)  {
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql = "insert into User (name, password, state) values(?,?,?)";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    stmt->bind(1, user.getName());
    stmt->bind(2, user.getPwd());
    stmt->bind(3, user.getState());
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    user.setId(mysql->getLastInsertId());
    return true;
}

User UserManager::query(userID id) {
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql = "select * from User where id = ?";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return User();
    }
    stmt->bind(1, id);
    auto stmtRes = stmt->query();
    while(stmtRes->next()) {
        User user;
        std::cout << "stmt getint " << stmtRes->getInt64(0) << std::endl;
        user.setId(stmtRes->getInt64(0));
        user.setName(stmtRes->getString(1));
        user.setPwd(stmtRes->getString(2));
        user.setState(stmtRes->getString(3));
        return user;
    }
    return User();
}

bool UserManager::updateState(User& user) {
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql = "update User set state = ? where id = ?";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    stmt->bind(1, user.getState());
    stmt->bind(2, user.getId());
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    return true;
}

void UserManager::resetState() {
    MySQL::ptr mysql = std::make_shared<MySQL>(params_);
    mysql->connect();
    std::string sql = "update User set state = 'offline' where state = 'online'";
    mysql->execute(sql);
}
