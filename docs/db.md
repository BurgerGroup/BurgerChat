## 表结构设计

创建数据库 : burgerChat

并且将数据库的编码设置为utf8，便于存放中文

DROP DATABASE IF EXISTS burgerChat;
CREATE DATABASE burgerChat DEFAULT CHARACTER SET utf8;

## User表

作为一个聊天系统，我们的服务器端肯定要有用户的信息，比如说账号，用户名，密码等。

在登录的时候，我们可以查询这个表里面的信息对用户身份进行验证，在注册的时候，我们则可以往表里面去写入数据。

