# 定义基本目录和编译器
WORKSPACE_ROOT := .
BIN_DIR        := $(WORKSPACE_ROOT)/bin
SRC_DIR        := $(WORKSPACE_ROOT)/src
LIBS_SRC_DIR   := $(WORKSPACE_ROOT)/libs

CXX      := g++
CXXFLAGS := -std=c++17 -I$(LIBS_SRC_DIR) -I/home/zsy/Documents/zsy/github/skia
LDFLAGS  := -L/home/zsy/Documents/zsy/github/skia/out/Static
LDLIBS   := -lskia -lX11 -lpthread -ldl -lm -ljpeg -lpng -lz -lwebp -lwebpdemux

# 自动收集所有源文件
# 主文件通过 MAIN 变量指定（默认编译 src/main.cpp）
MAIN        ?= $(SRC_DIR)/main.cpp
LIBS_SRCS   := $(wildcard $(LIBS_SRC_DIR)/*.cpp)
SOURCES     := $(MAIN) $(LIBS_SRCS)
OBJECTS     := $(SOURCES:.cpp=.o)

# 生成的可执行文件名（基于主文件名）
TARGET_NAME := $(notdir $(basename $(MAIN)))
TARGET      := $(BIN_DIR)/$(TARGET_NAME)

# 默认目标：编译并运行
.PHONY: all
all: build_and_run

# 编译并运行
.PHONY: build_and_run
build_and_run: $(TARGET)
	@echo "\n--- Running $(TARGET) ---"
	@$(TARGET)

# 生成可执行文件
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@
	@echo "\nBuilt target: $@"

# 编译每个 .cpp 为 .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 创建 bin 目录
$(BIN_DIR):
	mkdir -p $@

# 清理生成的文件
.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Cleaned all build files."
