// dns.h
#ifndef DNS_H
#define DNS_H

#include <stdint.h>
#include <netinet/in.h>

#define DNS_PORT 53
#define DNS_QUERY 0x0000
#define DNS_RESPONSE 0x8000

// DNS标头结构
typedef struct {
    uint16_t id;        // 事务 ID
    uint16_t flags;     // 标志
    uint16_t qdcount;   // 查询数量
    uint16_t ancount;   // 回答数量
    uint16_t nscount;   // 授权数量
    uint16_t arcount;   // 附加数量
} dns_header_t;

// DNS查询类型
typedef enum {
    DNS_TYPE_A = 1,
    DNS_TYPE_CNAME = 5,
    DNS_TYPE_AAAA = 28,
    DNS_TYPE_SRV = 33
} dns_query_type_t;

// DNS查询结构
typedef struct {
    const char *name;   // 查询域名
    dns_query_type_t type; // 查询类型
} dns_query_t;

// DNS响应记录结构
typedef struct {
    char *name;        // 记录名称
    uint16_t type;     // 记录类型
    uint16_t clazz;    // 类（通常为 1）
    uint32_t ttl;      // 生存时间
    uint16_t rdlength; // 数据长度
    void *rdata;       // 记录数据
} dns_record_t;

// 函数声明
void init_dns_request(dns_header_t *header, dns_query_t *query);
void send_dns_query(const char *domain, dns_query_type_t type);
void parse_dns_response(const uint8_t *response, size_t length);

#endif // DNS_H
