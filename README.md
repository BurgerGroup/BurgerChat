<div align="center">

<img src="./docs/assets/logo.jpeg" width = "100" height = "80" alt="burger" align=center />

BurgerChat : c++ terminal IM based on Burger, ncurses, redis and nginx

![](https://img.shields.io/badge/release-v1.0-blue.svg)
![](https://img.shields.io/badge/build-passing-green.svg)
![](https://img.shields.io/badge/dependencies-up%20to%20date-green.svg)
![](https://img.shields.io/badge/license-MIT-blue.svg)

</div>

-----

## ✨ 项目介绍

一个基于ncurses和Burger的terminal通讯软件，主要业务分为注册、登录、加好友、查看离线消息、一对一聊天等

todo: 业务流程图

## ⌛️ 构建

```
$ sudo apt install g++ cmake make libboost-all-dev mysql-server libmysqlclient-dev libcurl4-openssl-dev 
 
$ git clone https://github.com/BurgerGroup/BurgerChat.git

$ cd Burger

$ mkdir build && cmake ..

$ make 

$ make install   
```

## 🥇 业务流程

### 注册业务 []