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

## 复合主键和联合主键

https://www.liaoxuefeng.com/wiki/1177760294764384/1218728391867808

选取主键的一个基本原则是：不使用任何业务相关的字段作为主键。

## mysql中 的 ENGINE = innodb;

存储引擎是innodb。innoDB 是 MySQL 上第一个提供外键约束的数据存储引擎，除了提供事务处理外，InnoDB 还支持行锁，提供和 Oracle 一样的一致性的不加锁读取，能增加并发读的用户数量并提高性能，不会增加锁的数量。InnoDB 的设计目标是处理大容量数据时最大化性能，它的 CPU 利用率是其他所有基于磁盘的关系数据库引擎中最有效率的。

InnoDB 是一套放在 MySQL 后台的完整数据库系统，InnoDB 有它自己的缓冲池，能缓冲数据和索引，InnoDB 还把数据和索引存放在表空间里面，可能包含好几个文件，这和 MyISAM 表完全不同，在 MyISAM 中，表被存放在单独的文件中，InnoDB 表的大小只受限于操作系统文件的大小，一般为 2GB