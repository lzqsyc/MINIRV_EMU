#include "../head/head.h"


// 清理行内容
 void clean_line(char *line) {
    // 移除换行符
    line[strcspn(line, "\n")] = 0;
    line[strcspn(line, "\r")] = 0;
    
    // 移除行尾注释
    char *comment = strchr(line, '#');
    if (comment) *comment = 0;
    
    // 移除前后空白字符
    char *start = line;
    while (isspace((unsigned char)*start)) start++;
    
    char *end = line + strlen(line) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
    
    // 移动清理后的内容到行首
    if (start != line) {
        memmove(line, start, strlen(start) + 1);
    }
}
