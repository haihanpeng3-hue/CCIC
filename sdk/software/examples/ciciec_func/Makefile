TARGET = ciciec_func
HAS_CACHE ?= 1
ITERATIONS ?= 1000

C_SRCS := \
	./core_main.c \
	../coremark/core_list_join.c \
	../coremark/core_matrix.c \
	../coremark/core_state.c \
	../coremark/core_util.c \
	../coremark/core_portme.c

CFLAGS := -O3 -funroll-all-loops -finline-limit=200 -ftree-dominator-opts -fno-if-conversion2 -fselective-scheduling -fno-code-hoisting -fno-common -falign-functions=4 -falign-jumps=4 -falign-loops=4

# FPGA 上板默认使用真实串口波特率
CFLAGS += -DSIMU=0

CFLAGS += -DFLAGS_STR=\""$(CFLAGS)"\"
CFLAGS += -g
CFLAGS += -DITERATIONS=$(ITERATIONS)

OBJDIR = obj
COMMON_DIR = ../../bsp
GCC_DIR=../../../toolchains/loongson-gnu-toolchain-8.3-x86_64-loongarch32r-linux-gnusf-v2.0
PICOLIBC_DIR=../../../toolchains/picolibc
INCLUDES += -I../coremark
include ../../bsp/common.mk
