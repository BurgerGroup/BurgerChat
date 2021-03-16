## 表结构设计

创建数据库 : burgerChat

并且将数据库的编码设置为utf8，便于存放中文

DROP DATABASE IF EXISTS burgerChat;
CREATE DATABASE burgerChat DEFAULT CHARACTER SET utf8;

## 表与表之间的关系

表与表之间的关系，基本上是3种

- 一对一
- 一对多
- 多对一

那么怎么判断关系到底是什么呢？

## 建表顺序

在建表过程中，会设置外键约束，所以表和表之间有依赖关系。 因此会先建立被外键指向的表，然后再是其他表

