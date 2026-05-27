# ciciec_func

从 `CCIC2025` 迁移过来的综合验证程序。

功能：

- 运行 CoreMark 并输出串口结果
- 将 `CoreMark/MHz * 10` 显示到数码管
- 持续读取拨码开关并镜像到 LED

编译：

```sh
make clean
make
```

该示例复用 `../coremark/` 的官方 CoreMark 源码，只在本目录保留 2025 版的板级演示 `core_main.c`。
