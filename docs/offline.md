## 离线消息

```sql
CREATE table OfflineMsg(
    userid BIGINT NOT NULL,
    message VARCHAR(500) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

```

```cpp
class OfflineMsgManager {
public:
    OfflineMsgManager();
    ~OfflineMsgManager() = default;
    bool add(UserId userid, std::string msg); // 存储用户的离线消息
    bool remove(UserId userid); // 删除用户的离线消息
    std::vector<std::string> query(UserId userid); // 查询用户的离线消息
private:
    std::map<std::string, std::string> params_;
};

```




