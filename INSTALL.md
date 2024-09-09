### `INSTALL.md`

```markdown
# 安装指南

本文件提供了如何安装和配置简单 DNS 服务器的详细说明。

## 系统要求

- 操作系统: Linux（或其他兼容 POSIX 的系统）
- 编译器: GCC
- 根权限或使用 `sudo` 的能力

## 安装步骤

### 1. 克隆项目

首先，将项目从版本控制系统（如 Git）克隆到本地。打开终端并执行以下命令：

```bash
git clone <your-repo-url>
cd <your-repo-directory>
```

请将 `<your-repo-url>` 和 `<your-repo-directory>` 替换为实际的 Git 仓库 URL 和目录名称。

### 2. 安装依赖

此项目依赖于 GCC 编译器。如果您的系统尚未安装 GCC，可以使用以下命令安装：

#### 对于 Debian/Ubuntu 系统:

```bash
sudo apt update
sudo apt install build-essential
```

#### 对于 CentOS/RHEL 系统:

```bash
sudo yum groupinstall 'Development Tools'
```

### 3. 编译项目

使用 `make` 命令编译项目。这将根据 `Makefile` 中的规则生成可执行文件：

```bash
make
```

### 4. 配置服务器

在项目根目录中创建一个名为 `server.conf` 的配置文件，格式如下：

```
PORT=53
LOG_FILE=dns_server.log
```

- `PORT`: 设置 DNS 服务器监听的端口（默认是 53，用于 DNS）。
- `LOG_FILE`: 设置日志文件的名称（默认是 `dns_server.log`）。

### 5. 运行服务器

要运行服务器，需要超级用户权限。使用以下命令启动 DNS 服务器：

```bash
sudo make run
```

服务器启动后，将会监听指定端口并处理 DNS 查询。

### 6. 清理生成的文件（可选）

要删除生成的可执行文件和日志文件，可以运行：

```bash
make clean
```

## 测试

如果需要进行测试，可以使用以下命令（若有提供测试逻辑）：

```bash
make test
```

## 常见问题

如在安装或运行过程中遇到问题，请检查以下事项：

- 确保已安装所有必需的依赖。
- 检查程序是否有足够的权限监听指定端口（通常端口 53 需要超级用户权限）。
- 查看日志文件以获取更多错误信息。

如有其他问题，请参考 [README.md](README.md) 文件或提出 issue。