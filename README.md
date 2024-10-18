### `README.md`

```markdown
# 简单 DNS 服务器

这是一个简单的 DNS 服务器实现，使用 C 语言编写。该项目展示了基本的 DNS 协议处理逻辑，并通过 UDP 套接字进行通信。

## 特性

- 支持基本的 DNS 查询
- UDP 套接字通信
- 配置文件支持
- 日志记录功能

## 文件结构

```
your_project_directory/
│
├── Makefile          # 编译和构建项目的规则
├── dns_server.c      # DNS 服务器核心逻辑实现
├── dns_server.h      # DNS 服务器头文件，包含函数声明和宏定义
└── main.c            # 启动 DNS 服务器的主函数
```

## 编译和运行

### 要求

- GCC 编译器
- Linux 环境（或支持 POSIX 的系统）

### 安装

1. 克隆这个库：
   ```bash
   git clone <your-repo-url>
   cd <your-repo-directory>
   ```

2. 编译代码：
   ```bash
   make
   ```

3. 运行服务器（需要超级用户权限）：
   ```bash
   sudo make run
   ```

### 配置

可以通过文件 `server.conf` 来修改服务器配置。示例配置文件格式如下：

```
PORT=53
LOG_FILE=dns_server.log
```

- `PORT`: 设置 DNS 服务器监听的端口（默认是 53）。
- `LOG_FILE`: 设置日志文件的名称（默认是 `dns_server.log`）。

### 清理

要删除生成的可执行文件和日志文件，可以运行：
```bash
make clean
```

### 测试

运行测试逻辑（如果有）：
```bash
make test
```

## 日志记录

服务器的运行状态和错误日志将被记录在指定的日志文件中，便于调试和监控。

## 贡献

欢迎对本项目提出建议或贡献代码，请提交 issue 或 pull request。

## 许可证

该项目采用 MIT 许可证，请参见 [LICENSE](LICENSE) 文件以获取详细信息。

**💰 Donation Links:**
#### Donate Links

<b>BTC</b>: <code>39yp6fdcCiSn4v7d9JQAN27DffnMnUknwJ</code></br>
<b>BTC</b>: <code>bc1q80kznf4nzt2ratzctc6m3d8xw8avxw7rlq06rh</code></br>
<b>BTC</b>: <code>36EojEJBkLuEMC8hpNFhJJ8tuBtaX8vU3m</code></br></br>