## 手写json模拟客户端测试后台

## 数据库操作

service mysql status

service mysql start

mysql -u root -p

UPDATE User SET state='offline' WHERE id=3;

desc OfflineMsg;

## 测试注册

{"msgid":4, "name":"yyy", "password":"123123"}

return {"errno":0,"id":17,"msgid":5}

select * from User; 查看

## 测试登录

{"msgid":1, "id":3, "password":"123123"}

return {"errno":0,"id":3,"msgid":2,"name":"yyy"}

## 测试点对点通信

再开一个客户端

{"msgid":1, "id":2, "password":"1312"}

return {"errno":0,"id":2,"msgid":2,"name":"lighao"}

select * from User; 查看是否变online

yyy 对 lighao 说

{"msgid":6, "id":3, "from":"yyy", "to":2, "msg":"hello ligh"}

然后中转过去了

## 测试离线消息

先登录一个账户

{"msgid":1, "id":3, "password":"123123"}

另一个账户处于offline状态

yyy 对 lighao 说

{"msgid":6, "id":3, "from":"yyy", "to":2, "msg":"hello ligh111"}

{"msgid":6, "id":3, "from":"yyy", "to":2, "msg":"hello ligh222"}

select * from OfflineMsg; 查看

然后再开启一个客户端，登陆lighao

{"msgid":1, "id":2, "password":"1312"}

看是否转发过来

## 测试服务器异常退出

先登录一个账户

{"msgid":1, "id":3, "password":"123123"}

然后ctrl+c 断开服务器

select * from User; 查看是否变offline

## 添加好友测试

