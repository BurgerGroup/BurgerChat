## 离线提醒

```sql
CREATE table OfflineNotification(
    userid BIGINT NOT NULL,
    fromid BIGINT NOT NULL,
    recvTime BIGINT NOT NULL,
    fromType ENUM('user', 'group') DEFAULT 'user',
    message VARCHAR(500) NOT NULL,
    PRIMARY KEY(userid, fromid, fromType, recvTime)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```

```cpp

class OfflineNotificationManager {
public:
    OfflineNotificationManager();
    ~OfflineNotificationManager() = default;
    void add(UserId userid, UserId fromid, uint64_t recvTime, 
            const std::string& fromType = "user", const std::string& msg = ""); // 存储用户的离线好友通知

    void remove(UserId userid, UserId fromid, uint64_t recvTime, 
            const std::string& fromType = "user"); // 删除用户的离线通知
            
    // std::vector<std::string> query(UserId userid, UserId fromid, const std::string& fromType = "user"); // 查询用户的离线通知
    std::vector<std::string> query(UserId userid);
private:
    std::map<std::string, std::string> params_;
};

```