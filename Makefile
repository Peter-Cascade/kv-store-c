# 自动找到 Makefile 所在的目录作为项目根
ROOT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

CC = gcc
# 调试模式：带 -g，不优化（-O0），打开所有警告
CFLAGS = -Wall -Wextra -g -O0 -I$(ROOT_DIR)include

# 发布模式（make RELEASE=1 时使用）
ifeq ($(RELEASE),1)
CFLAGS = -Wall -Wextra -O2 -DNDEBUG -I$(ROOT_DIR)include
endif

SRC = $(wildcard $(ROOT_DIR)src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = kv_store
BUILD_DIR = $(ROOT_DIR)build

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJ)
	@echo "✓ 编译完成: $@"

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

DEPS = $(ROOT_DIR)include/kv.h

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# 运行
run: all
	$(BUILD_DIR)/$(TARGET)

# 用 gdb 调试（自动设断点在 main，可以直接 run）
debug: all
	gdb -ex run $(BUILD_DIR)/$(TARGET)

# 用 valgrind 检查内存
memcheck: all
	valgrind --leak-check=full --show-leak-kinds=all $(BUILD_DIR)/$(TARGET)

# 清理
clean:
	rm -rf $(BUILD_DIR) $(OBJ)
	@echo "✓ 已清理"

# 重新编译
rebuild: clean all

# 发布版本（优化 + 去掉断言）
release: clean
	$(MAKE) RELEASE=1

.PHONY: all clean run rebuild debug memcheck release