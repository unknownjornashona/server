// dns.c
#include "dns.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// 初始化 DNS 请求
void init_dns_request(dns_header_t *header, dns_query_t *query) {
    header->id = (uint16_t)rand(); // 随机生成 ID
    header->flags = htons(DNS_QUERY); // 设置查询标志
    header->qdcount = htons(1); // 查询数量为 1
    header->ancount = 0; // 初始回答数量为 0
    header->nscount = 0; // 初始授权数量为 0
    header->arcount = 0; // 初始附加数量为 0
}

// 发送 DNS 查询
void send_dns_query(const char *domain, dns_query_type_t type) {
    int sockfd;
    struct sockaddr_in server_addr;
    uint8_t buffer[512]; // DNS 数据包缓冲区
    dns_header_t *header = (dns_header_t *)buffer;
    dns_query_t query = { domain, type };

    // 创建 UDP 套接字
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 设置 DNS 服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DNS_PORT);
    server_addr.sin_addr.s_addr = inet_addr("8.8.8.8"); // Google DNS

    // 初始化 DNS 请求
    init_dns_request(header, &query);
    
    // 构造 DNS 查询部分
    size_t offset = sizeof(dns_header_t);
    char *token = strtok((char *)query.name, ".");
    while (token) {
        size_t len = strlen(token);
        buffer[offset++] = (uint8_t)len;
        memcpy(&buffer[offset], token, len);
        offset += len;
        token = strtok(NULL, ".");
    }
    buffer[offset++] = 0; // Name 结束标记
    *(uint16_t *)&buffer[offset] = htons(query.type); // 查询类型
    offset += sizeof(uint16_t);
    *(uint16_t *)&buffer[offset] = htons(1); // 类别 IN
    offset += sizeof(uint16_t);

    // 发送 DNS 查询
    ssize_t send_len = sendto(sockfd, buffer, offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (send_len < 0) {
        perror("Send failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("DNS query sent for %s\n", query.name);

    // 接收 DNS 响应
    socklen_t addr_len = sizeof(server_addr);
    ssize_t recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, &addr_len);
    if (recv_len < 0) {
        perror("Receive failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 解析 DNS 响应
    parse_dns_response(buffer, recv_len);

    // 关闭套接字
    close(sockfd);
}

// 解析 DNS 响应
void parse_dns_response(const uint8_t *response, size_t length) {
    const dns_header_t *header = (const dns_header_t *)response;

    // 检查响应
    if (ntohs(header->flags) & DNS_RESPONSE) {
        printf("Received DNS response with ID: %u\n", ntohs(header->id));
        uint16_t answer_count = ntohs(header->ancount);
        printf("Answer Count: %u\n", answer_count);
        
        const uint8_t *ptr = response + sizeof(dns_header_t);
        for (uint16_t i = 0; i < answer_count; i++) {
            // 解析记录名称
            while (*ptr > 0) {
                ptr += *ptr + 1; // 遍历名称
            }
            ptr++; // 跳过名称结束标记
            
            uint16_t type = ntohs(*(uint16_t *)ptr);
            ptr += 2; // 跳过类型
            ptr += 2; // 跳过类
            ptr += 4; // 跳过TTL
            uint16_t rdlength = ntohs(*(uint16_t *)ptr);
            ptr += 2; // 跳过数据长度

            // 读取记录数据
            if (type == DNS_TYPE_A) {
                struct in_addr addr;
                memcpy(&addr, ptr, sizeof(addr));
                printf("A record: %s\n", inet_ntoa(addr));
            }
            ptr += rdlength; // 移动指针
        }
    } else {
        printf("Not a valid DNS response.\n");
    }
}
