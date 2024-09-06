#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFSIZE 512
#define DEFAULT_PORT 53
#define DEFAULT_LOG_FILE "dns_server.log"

// 配置结构体
typedef struct {
    int port;
    char log_file[256];
} Config;

// 日志记录函数
void log_message(const char *message, const char *log_file) {
    FILE *file = fopen(log_file, "a");
    if (file) {
        fprintf(file, "%s\n", message);
        fclose(file);
    }
}

// 读取配置文件
Config read_config(const char *filename) {
    Config config;
    config.port = DEFAULT_PORT; // 默认端口
    strcpy(config.log_file, DEFAULT_LOG_FILE); // 默认日志文件

    FILE *file = fopen(filename, "r");
    if (!file) {
        log_message("配置文件未找到，使用默认配置", config.log_file);
        return config; // 返回默认配置
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "PORT=", 5) == 0) {
            config.port = atoi(line + 5);
        } else if (strncmp(line, "LOG_FILE=", 9) == 0) {
            strncpy(config.log_file, line + 9, sizeof(config.log_file) - 1);
            config.log_file[strcspn(config.log_file, "\n")] = 0; // 去掉换行符
        }
    }

    fclose(file);
    return config;
}

void buildResponse(unsigned char *buf, int queryLen) {
    // DNS 响应格式设置
    buf[2] = 0x81; // 标识符
    buf[3] = 0x80; // 响应标志
    buf[5] = 1;    // 有一个回答

    // 设置回答的资源记录
    int answerOffset = queryLen + 11;
    buf[answerOffset] = 0xc0; // 指向查询的位置
    buf[answerOffset + 1] = 0x0c; // 名称的长度

    buf[answerOffset + 2] = 0x00; // 类型 A
    buf[answerOffset + 3] = 0x01; // 类型 A
    buf[answerOffset + 4] = 0x00; // 类别 IN
    buf[answerOffset + 5] = 0x01; // 类别 IN
    buf[answerOffset + 6] = 0x00; // TTL
    buf[answerOffset + 7] = 0x00; // TTL
    buf[answerOffset + 8] = 0x00; // TTL
    buf[answerOffset + 9] = 0x00; // TTL

    buf[answerOffset + 10] = 0x04; // 数据长度
    buf[answerOffset + 11] = 0x7f; // IP 地址
    buf[answerOffset + 12] = 0x00; // 127.0.0.1
    buf[answerOffset + 13] = 0x00; // 127.0.0.1
    buf[answerOffset + 14] = 0x01; // 127.0.0.1
}

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in server, client;
    unsigned char buffer[BUFSIZE];
    socklen_t clientLen = sizeof(client);

    // 读取配置文件
    Config config = read_config("server.conf");

    // 创建 UDP 套接字
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        handle_error("无法创建套接字");
    }

    // 设置服务器地址
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(config.port);

    // 绑定套接字
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        handle_error("绑定失败");
    }

    printf("DNS 服务器已启动，正在监听端口 %d\n", config.port);
    log_message("DNS 服务器启动", config.log_file);

    while (1) {
        int queryLen = recvfrom(sockfd, buffer, BUFSIZE, 0, (struct sockaddr *)&client, &clientLen);
        if (queryLen < 0) {
            log_message("接收查询失败", config.log_file);
            continue;
        }

        log_message("接收到查询", config.log_file);

        buildResponse(buffer, queryLen);

        // 发送响应
        if (sendto(sockfd, buffer, queryLen + 15, 0, (struct sockaddr *)&client, clientLen) < 0) {
            log_message("发送响应失败", config.log_file);
        } else {
            log_message("成功发送响应", config.log_file);
        }
    }

    close(sockfd);
    return 0;
}
