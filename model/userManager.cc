#include "userManager.h"

// todo : 优化 ：抽个类出来

UserManager::UserManager() {
    params_["host"] = Config::Instance().getString("mysql", "host", "127.0.0.1");
    params_["user"] = Config::Instance().getString("mysql", "user", "root");
    params_["passwd"] = Config::Instance().getString("mysql", "password", "PWD");
    params_["dbname"] = Config::Instance().getString("mysql", "dbname", "burgerChat");
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

User UserManager::query(UserId id) {
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
