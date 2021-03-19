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

## todo 

1. database 交互需要思考下
2. ImPlot：用于分析性能