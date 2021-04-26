## 运行

```bash
docker run -itd --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -p 9000:22 -p 9001:9001 -v /Users/frank/Documents/pro:/home/pro burger:v1 /bin/bash
```

## 架构

网络 + 业务 + 数据(db)


## FAQ

## 如何gdb ncurses 界面交互

https://stackoverflow.com/questions/11965328/debugging-ncurses-application-with-gdb

https://stackoverflow.com/questions/9617364/using-cgdb-for-a-ncurses-c-application

## 一对一聊天

```
msgid 
id : 1 # myid
from : "frank"
to : 3 # friend's id
msg : "xxx"
```

## todo 

1. database 交互需要思考下
2. ImPlot：用于分析性能


## problems

* 1. 怎么让各个线程各司其职：线程分工不清晰的后果就是状态转移错乱（包括读取消息、退出）
* 2. 状态的变化和函数的调用流程：登录的状态转移设计。
* 3. 表的设计：怎么才能不冗余？怎么能够快速的确定需要的目标？怎么快速的进行各种状态的改变？
* 4. 延后处理：很多消息不会及时处理，那么怎么保证能够安全完整地保存下来？