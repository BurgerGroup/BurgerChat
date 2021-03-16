#include "userManager.h"

// todo : 优化 ：抽个类出来

UserManager::UserManager(std::string host, std::string user,
                    std::string passwd, std::string dbname) {
    params["host"] = host;
    params["user"] = user;
    params["passwd"] = passwd;
    params["dbname"] = dbname;
}

bool UserManager::add(User &user)  {
    MySQL::ptr mysql = std::make_shared<MySQl>(parmas_);
    mysql->connect();
    std::string sql = "insert into user (name, password, state) values(?,?,?)";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    stmt->bind(1, user.getName());
    stmt->bind(2, user.getPwd());
    stmt->bind(2, user.getState());
    if(stmt->execute()) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    user.setId(mysql->getLastInsertId());
    return true;
}

User UserManager::query(int id) {
    MySQL::ptr mysql = std::make_shared<MySQl>(parmas_);
    mysql->connect();
    std::string sql = "select * from user where id = ?";
    auto stmt = mysql->prepare(sql);
    if(!stmt) {
        ERROR("stmt = {} errno = {} errstr = {}", sql, mysql->getErrno(), mysql->getErrStr());
        return false;
    }
    stmt->bind(1, user.getId());
    auto stmtRes = stmt->query();
    while(stmtRes->next()) {
        User user;
        user.setId(stmtRes->getInt32(0));
        user.setName(stmtRes->getString(1));
        user.setPwd(stmtRes->getString(2));
        user.setState(stmtRes->getString(3));
        return true;
    }
    return false;
}

bool UserManager::updateState(User user) {
    MySQL::ptr mysql = std::make_shared<MySQl>(parmas_);
    mysql->connect();
    std::string sql = "update user set state = ? where id = ?";
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
    MySQL::ptr mysql = std::make_shared<MySQl>(parmas_);
    mysql->connect();
    std::string sql = "update user set state = 'offline' where state = 'online'";
    mysql->execute(sql);
}
