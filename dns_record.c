// dns_record.c
#include "dns_record.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 全局配置变量
int MAX_RECORDS = 100; // 默认最大记录数
int DEFAULT_TTL = 3600; // 默认TTL

// 全局的DNS记录列表
dns_record_t *records[MAX_RECORDS];

// 配置文件读取
void load_config(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open config file");
        return;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char key[128];
        char value[128];

        if (sscanf(line, "%127[^=]=%127s", key, value) == 2) {
            if (strcmp(key, "MAX_RECORDS") == 0) {
                MAX_RECORDS = atoi(value);
            } else if (strcmp(key, "DEFAULT_TTL") == 0) {
                DEFAULT_TTL = atoi(value);
            }
        }
    }

    fclose(file);
}

// 创建A记录
dns_record_t *create_a_record(const char *name, const char *address, uint32_t ttl) {
    dns_record_t *record = malloc(sizeof(dns_record_t));
    if (record) {
        record->name = strdup(name);
        record->type = DNS_A;
        record->ttl = ttl;
        record->data.address = strdup(address);
    }
    return record;
}

// 创建CNAME记录
dns_record_t *create_cname_record(const char *name, const char *cname, uint32_t ttl) {
    dns_record_t *record = malloc(sizeof(dns_record_t));
    if (record) {
        record->name = strdup(name);
        record->type = DNS_CNAME;
        record->ttl = ttl;
        record->data.address = strdup(cname);
    }
    return record;
}

// 创建AAAA记录
dns_record_t *create_aaaa_record(const char *name, const char *ipv6_address, uint32_t ttl) {
    dns_record_t *record = malloc(sizeof(dns_record_t));
    if (record) {
        record->name = strdup(name);
        record->type = DNS_AAAA;
        record->ttl = ttl;
        record->data.ipv6_address = strdup(ipv6_address);
    }
    return record;
}

// 创建SRV记录
dns_record_t *create_srv_record(const char *name, const char *hostname, uint16_t port, uint32_t ttl) {
    dns_record_t *record = malloc(sizeof(dns_record_t));
    if (record) {
        record->name = strdup(name);
        record->type = DNS_SRV;
        record->ttl = ttl;
        record->data.srv.hostname = strdup(hostname);
        record->data.srv.port = port;
    }
    return record;
}

// 释放DNS记录
void free_dns_record(dns_record_t *record) {
    if (record) {
        free(record->name);
        switch (record->type) {
            case DNS_A:
            case DNS_CNAME:
                free(record->data.address);
                break;
            case DNS_AAAA:
                free(record->data.ipv6_address);
                break;
            case DNS_SRV:
                free(record->data.srv.hostname);
                break;
        }
        free(record);
    }
}

// 打印DNS记录
void print_dns_record(const dns_record_t *record) {
    if (!record) {
        return;
    }
    printf("Record: %s (%d), TTL: %u\n", record->name, record->type, record->ttl);
    switch (record->type) {
        case DNS_A:
        case DNS_CNAME:
            printf("Address: %s\n", record->data.address);
            break;
        case DNS_AAAA:
            printf("IPv6 Address: %s\n", record->data.ipv6_address);
            break;
        case DNS_SRV:
            printf("SRV: %s:%u\n", record->data.srv.hostname, record->data.srv.port);
            break;
    }
}

// 查找DNS记录的函数
dns_record_t *find_dns_record(const char *name, dns_record_type type) {
    for (int i = 0; i < MAX_RECORDS; i++) {
        if (records[i] != NULL && strcmp(records[i]->name, name) == 0 && records[i]->type == type) {
            return records[i];
        }
    }
    return NULL; // 找不到记录
}

// 示例查询处理函数
void handle_query(const char *query) {
    // 假设我们获取了查询的记录类型和名称
    dns_record_type type = DNS_A; // 可以改为从查询中解析
    const char *name = query;

    dns_record_t *record = find_dns_record(name, type);
    if (record) {
        print_dns_record(record);
    } else {
        printf("Record not found.\n");
    }
}

// 添加记录的示例函数
void add_records() {
    // 示例，创建几条DNS记录
    records[0] = create_a_record("example.com", "192.0.2.1", DEFAULT_TTL);
    records[1] = create_cname_record("www.example.com", "example.com", DEFAULT_TTL);
    records[2] = create_aaaa_record("ipv6.example.com", "2001:0db8::1", DEFAULT_TTL);
    records[3] = create_srv_record("_sip._tcp.example.com", "sipserver.example.com", 5060, DEFAULT_TTL);
}

// 主函数示例
int main(int argc, char *argv[]) {
    // 读取配置
    load_config("server.conf");

    // 添加记录
    add_records();

    // 处理查询示例
    handle_query("example.com");

    // 释放所有记录
    for (int i = 0; i < MAX_RECORDS; i++) {
        free_dns_record(records[i]);
    }

    return 0;
}
