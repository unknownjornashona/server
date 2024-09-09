// dns_record.h
#ifndef DNS_RECORD_H
#define DNS_RECORD_H

#include <stdint.h>

#define MAX_RECORDS 100  // 默认最大记录数，可以在运行时从配置读取

// DNS记录类型
typedef enum {
    DNS_A,       // A记录
    DNS_CNAME,   // CNAME记录
    DNS_AAAA,    // AAAA记录
    DNS_SRV      // SRV记录
} dns_record_type;

// DNS记录结构
typedef struct {
    char *name;          // 记录名称
    dns_record_type type; // 记录类型
    uint32_t ttl;       // 生存时间（TTL）
    union {
        char *address;   // A和CNAME记录的地址
        char *ipv6_address; // AAAA记录的IPv6地址
        struct {
            char *hostname; // SRV记录的主机名
            uint16_t port;   // SRV记录的端口号
        } srv;
    } data;
} dns_record_t;

// 函数声明
dns_record_t *create_a_record(const char *name, const char *address, uint32_t ttl);
dns_record_t *create_cname_record(const char *name, const char *cname, uint32_t ttl);
dns_record_t *create_aaaa_record(const char *name, const char *ipv6_address, uint32_t ttl);
dns_record_t *create_srv_record(const char *name, const char *hostname, uint16_t port, uint32_t ttl);
void free_dns_record(dns_record_t *record);
void print_dns_record(const dns_record_t *record);
void load_config(const char *filename); // 新增的函数声明

#endif // DNS_RECORD_H
