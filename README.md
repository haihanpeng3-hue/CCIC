# CCIC2026 LoongArch SoC Project

2026 年中国大学生集成电路创新创业大赛（龙芯中科企业命题）初赛工程仓库。

本项目基于命题方提供的发布包，在 `CCIC2026` 基础上完成了初赛要求中的 SoC 集成、软件编译与仿真验证流程整理，目标是让仓库保持“可复现、可说明、可提交”的状态。

## 当前完成情况

### RTL / SoC 集成

- 补齐 `rtl/soc_top.v` 中的关键主链：
  - `core_top`
  - `Axi_CDC`
  - `axi_wrap_ram_sp_external`
  - `axi_uart_controller`
  - `axi_dvi`
  - `confreg`
- 完成 `confreg_int` 到 CPU 中断输入的同步与接入。
- 在 `rtl/ip/confreg/confreg.v` 中实现外部中断控制逻辑：
  - `int_en`
  - `int_edge`
  - `int_pol`
  - `int_clr`
  - `int_set`
  - `int_state`

### 软件与功能程序

- `sdk/software/examples/hello_world`
- `sdk/software/examples/int_test`
- `sdk/software/examples/pinball_game`

以上三个示例已经在 Docker 交叉编译环境中成功生成 `.elf/.bin/.mif`。

### 验证状态

- Vivado 工程可由 `fpga/create_project.tcl` 正常创建。
- 仿真日志显示 `pinball_game` 已进入按键选时流程。
- 综合、实现日志已跑通，无阻塞性错误。

## 目录说明

```text
CCIC2026/
├─ doc/         赛题说明与整理后的文档
├─ fpga/        Vivado 工程脚本与约束
├─ rtl/         SoC 顶层与各 IP RTL
├─ sdk/         软件示例、BSP 与交叉工具链入口
└─ sim/         行为仿真 testbench 与 SRAM 模型
```

## 推荐开发环境

### 1. Vivado

- 推荐版本：**Vivado 2019.2**（与发布包 IP 版本更一致）
- 当前仓库已在 Vivado 2025.1 下完成过综合/实现验证，但不建议将自动升级后的 IP 产物直接入库。

### 2. Docker 交叉编译环境

已验证的容器名称：`loongson-env`

容器中需要可用：

- `make`
- `loongarch32r-linux-gnusf-gcc`
- `picolibc`

## 软件编译方法

如果 `CCIC2026/sdk/toolchains` 下未展开完整工具链，可以直接复用容器内已有的工具链目录覆盖变量：

```bash
docker exec -e LA32RSOC_WINDOWS_HOME=/workspace2026 \
  -w /workspace2026/sdk/software/examples/hello_world \
  loongson-env bash -ic \
  "make GCC_DIR=/workspace/sdk/toolchains/loongson-gnu-toolchain-8.3-x86_64-loongarch32r-linux-gnusf-v2.0 PICOLIBC_DIR=/workspace/sdk/toolchains/picolibc"
```

`int_test` 与 `pinball_game` 编译方式相同，只需替换工作目录。

## Vivado 工程创建

在 `fpga/` 目录下执行：

```tcl
vivado -mode batch -source create_project.tcl
```

脚本会自动：

- 创建 `Loongson_Soc` 工程
- 添加 `rtl/` 下源码
- 添加 `sim/` 下仿真文件
- 添加 `constraints/` 下约束文件

## 仿真现象说明

当前仿真日志中已经观测到如下输出：

```text
Please Choosetime !!
button1
button2
button3
Choosetime:1button4
```

这与 `pinball_game` 的按键选时逻辑基本一致：

- `button1`：增加延时
- `button2`：保留按键分支
- `button3`：确认并开始游戏
- `button4`：减小延时

尾部的 `button4` 更像是确认打印与 `chooseFlag` 清零之间的一个中断竞态窗口，不是中断链路失效。

## 建议提交内容

建议仓库只保留以下内容：

- RTL 源码
- 仿真 testbench
- 约束与 Vivado 创建脚本
- 软件源码与 BSP
- 文档说明

不建议提交：

- `fpga/project/`
- `fpga/.Xil/`
- `sdk/*.bin`
- `sdk/*.mif`
- `sdk/software/examples/*/obj/`

这些内容都可以由脚本或编译流程重新生成。

## 后续建议

如果要继续完善比赛交付，下一步建议补：

1. 初赛答辩/提交用的系统设计说明书
2. `pinball_game` 的 DVI 画面截图与 FPGA 实机录像
3. 综合与实现报告中的资源占用、时序结果整理
4. 仿真波形中断触发点与寄存器读写截图
