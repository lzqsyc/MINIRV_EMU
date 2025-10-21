CC = gcc
CHECK = -Wall -Wextra -std=c99 -g
INCLUDES = -Iinclude

# 目录结构定义
BIN_DIR = bin
BUILD_DIR = build
HEAD_DIR = head
TEST_DIR = test
LOAD_DIR = load
SRC_DIR = src

# 获取源文件
HEAD_FILE = $(wildcard $(HEAD_DIR)/*.h)
SRC_FILE = $(wildcard $(SRC_DIR)/*.c)
LOAD_FILE = $(wildcard $(LOAD_DIR)/*.c)
TEST_FILE = $(wildcard $(TEST_DIR)/*.c)
SRC = $(SRC_FILE) $(LOAD_FILE) $(TEST_FILE)


# 对象文件生成
SRC_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILE))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_FILE)) 
LOAD_OBJS = $(patsubst $(LOAD_DIR)/%.c,$(BUILD_DIR)/%.o,$(LOAD_FILE)) 

OBJS = $(SRC_OBJS) $(LOAD_OBJS) $(TEST_OBJS)
TARGET = $(BIN_DIR)/Minirv_emu

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CHECK) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEAD_FILE)
	$(CC) $(CHECK) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c $(HEAD_FILE)
	$(CC) $(CHECK) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(LOAD_DIR)/%.c $(HEAD_FILE)
	$(CC) $(CHECK) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) 
run: $(TARGET)
	./$(TARGET)
.PHONY:all clean run