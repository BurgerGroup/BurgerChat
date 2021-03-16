<div align="center">

<img src="./docs/assets/logo.jpeg" width = "100" height = "80" alt="burger" align=center />

BurgerChat : c++ terminal IM based on Burger, ncurses, redis and nginx

![](https://img.shields.io/badge/release-v1.0-blue.svg)
![](https://img.shields.io/badge/build-passing-green.svg)
![](https://img.shields.io/badge/dependencies-up%20to%20date-green.svg)
![](https://img.shields.io/badge/license-MIT-blue.svg)

</div>

-----

## Features:
- Network IO is based on the burger library
- Clean and easy to read c++ 
- Use burger's mysql wrapper api
- Interacting with objects and databases through the orm schema
- redis : Cluster Communication
- nginx : load balance

## Compile

```
$ sudo apt install g++ cmake make libboost-dev mysql-server libmysqlclient-dev

$ git clone https://github.com/chanchann/BurgerChat.git

$ cd BurgerChat

$ mkdir build && cmake ..

$ make 
```
