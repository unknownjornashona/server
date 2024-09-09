#include "dns_server.h"

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
