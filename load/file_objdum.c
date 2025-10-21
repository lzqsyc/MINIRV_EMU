#include "../head/head.h"

// 解析objdump格式的hex文件行
 int parse_objdump_line(const char *line, uint32_t *address, uint32_t instructions[8], int *count) {
    char *colon = strchr(line, ':');
    if (!colon) {
        return 0; // 不是objdump格式
    }
    
    // 解析地址部分 - 这是关键修复：将地址乘以4转换为字节地址
    char addr_str[32];
    strncpy(addr_str, line, colon - line);
    addr_str[colon - line] = '\0';
    
    char *endptr;
    uint32_t word_address = (uint32_t)strtoul(addr_str, &endptr, 16);
    if (endptr == addr_str) {
        return 0; // 地址解析失败
    }
    
    // 将字地址转换为字节地址（乘以4）
    *address = word_address * 4;
    
    // 解析数据部分
    const char *data_start = colon + 1;
    *count = 0;
    
    char data_copy[512];
    strncpy(data_copy, data_start, sizeof(data_copy) - 1);
    data_copy[sizeof(data_copy) - 1] = '\0';
    
    char *token = strtok(data_copy, " \t\n\r");
    while (token != NULL && *count < 8) {
        instructions[*count] = (uint32_t)strtoul(token, &endptr, 16);
        if (endptr == token) {
            return 0; // 数据解析失败
        }
        (*count)++;
        token = strtok(NULL, " \t\n\r");
    }
    
    return 1; // 成功解析
}

// 解析简单hex格式（每行一个指令）
 int parse_simple_hex_line(const char *line, uint32_t *instruction) {
    // 检查是否为有效的十六进制数
    for (const char *p = line; *p; p++) {
        if (!isxdigit((unsigned char)*p) && !isspace((unsigned char)*p)) {
            return 0; // 包含非十六进制字符
        }
    }
    
    // 解析十六进制数
    char *endptr;
    *instruction = (uint32_t)strtoul(line, &endptr, 16);
    
    if (endptr == line) {
        return 0; // 解析失败
    }
    
    // 允许行尾有空格
    while (*endptr != '\0' && isspace((unsigned char)*endptr)) {
        endptr++;
    }
    
    if (*endptr != '\0') {
        return 0; // 有额外的非空白字符
    }
    
    return 1;
}