
## 注册业务

作为一个聊天系统，我们的服务器端肯定要有用户的信息，比如说账号，用户名，密码等。 

在登录的时候，我们可以查询这个表里面的信息对用户身份进行验证，在注册的时候，我们则可以往表里面去写入数据。

首先我们看表设计

```sql
/* User表 */
CREATE table User(
    id BIGINT NOT NULL AUTO_INCREMENT,
    name VARCHAR(50) NOT NULL,
    password VARCHAR(50) NOT NULL,
    state ENUM('online', 'offline') DEFAULT 'offline',
    UNIQUE(name),
    PRIMARY KEY(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```

表对应的ORM类

```cpp
class User {
public:
    User(std::string name = "", std::string pwd = "", std::string state = "offline", UserId id = -1)
        : name_(name),
        password_(pwd),
        state_(state),
        id_(id) {
    }
    ~User() = default;
    void setId(UserId id) { id_ = id; }
    void setName(std::string name) { name_ = name; }
    void setPwd(std::string pwd) { password_ = pwd; }
    void setState(std::string state) { state_ = state; }

    UserId getId() const { return id_; }
    std::string getName() const { return name_; }
    std::string getPwd() const { return password_; }
    std::string getState() { return state_; }

protected:
    std::string name_;
    std::string password_;
    std::string state_;
    UserId id_;
};
```

对于User表的操作类

```cpp
class UserManager {
public:
    UserManager();
    ~UserManager() = default;
    bool add(User &user);
    User query(UserId id);
    bool updateState(User& user);
    void resetState();
private:
    std::map<std::string, std::string> params_;
};
```

## 流程

当我们前端发送过来json业务字符串时，

{"msgid":4, "name":"yyy", "password":"123123"}

我们可以看到msgid = 4,