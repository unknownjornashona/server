#ifndef DNS_SERVER_H
#define DNS_SERVER_H

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

// 函数声明
void log_message(const char *message, const char *log_file);
Config read_config(const char *filename);
void buildResponse(unsigned char *buf, int queryLen);
void handle_error(const char *msg);

#endif // DNS_SERVER_H
