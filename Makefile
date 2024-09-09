# Makefile for DNS Server

# 变量设置
CC = gcc
CFLAGS = -Wall -O2
SOURCES = main.c dns_server.c
HEADERS = dns_server.h
TARGET = simple_dns
CONFIG_FILE = server.conf
DIST_DIR = dist
DIST_TAR = $(DIST_DIR)/$(TARGET).tar.gz

# 文档文件
DOCS = COPYRIGHT.md CHANGELOG.md INSTALL.md README.md LICENSE

# 默认目标
all: $(TARGET)

# 编译目标
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

# 运行目标
run: $(TARGET)
	sudo ./$(TARGET)

# 清理生成的文件
clean:
	rm -f $(TARGET) dns_server.log

# 测试目标
test: $(TARGET)
	@echo "开始测试..."
	# 在这里添加测试逻辑
	# 例如，可以使用 curl 或其他工具测试响应
	@echo "测试完成."

# 创建分发包
dist: clean
	mkdir -p $(DIST_DIR)
	cp $(SOURCES) $(HEADERS) $(CONFIG_FILE) $(DOCS) $(DIST_DIR)/
	tar -czf $(DIST_TAR) -C $(DIST_DIR) .

# 清理分发文件
distclean:
	rm -rf $(DIST_DIR)

.PHONY: all run clean test dist distclean
