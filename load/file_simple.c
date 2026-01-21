#include "../head/head.h"

int simple_check(const char *line,uint32_t *instructions){
    // 经典字符串遍历 *p遇到‘\0’结束遍历
    for ( const char *p=line; *p; p++)
    {
        if( !isxdigit((unsigned char)*p) && !isspace((unsigned char)*p) ){
            return 0;
            }
    }
    /*
    const char *p = line
    while(*p){
        if( ！isxdigit((unsigned char)*p) && !isspace(（unsigned char）*p) ){
            return 0 ;
            }
        p++;
    }
    */
    char *endptr = NULL;
    *instructions=(uint32_t)strtoul(line,&endptr,16);
    if (endptr==line)
    {
        return 0;
    }
    // 检查行尾内容
    while (*endptr != '\0' && isspace((unsigned char) *endptr))
    {
        endptr++;
    }
    if (*endptr !='\0')
    {
        return 0;
    }
    return 1;
}

void load_simple(const char *filepath, CPU_state *cpu,int *line_count,int *inst_count,int *error_count){
    char line[128];
    uint32_t instruction;
     uint32_t load_addr = LOAD_START;
    int address_count = cpu->mem_size/4;
    int *address_used = ( int *)calloc(address_count,sizeof(int));
    if (address_used == NULL) return ;

    FILE *f = fopen(filepath, "r");
    if (f==NULL) return ;
    while (fgets(line, sizeof(line), f)){
        (*line_count)++;
        if (simple_check(line, &instruction)){
            if (load_addr +3 > cpu->mem_size){
                printf("警告: 第%d行 - 地址 0x%08x 超出内存范围，停止加载\n", *line_count, load_addr);
                break;
            }
            if (address_used[load_addr/4]){
                printf("警告: 第%d行 - 地址 0x%08x 已使用，跳过写入\n", *line_count, load_addr);
            (*error_count)++;
                load_addr +=4;
                continue;
            }
            mem_write(cpu,load_addr,instruction);
            address_used[load_addr/4]=1;
            if (*inst_count <10 ){
               printf("  [0x%08x] = 0x%08x\n", load_addr, instruction);
            }  else if (*inst_count == 10) {
                printf("  ... (后续指令加载信息省略)\n");
            }
            load_addr +=4;
            (*inst_count)++;
        }
        if (*line_count % 1000 ==0){
            printf("已处理 %d 行，加载 %d 条指令\n", *line_count, *inst_count);
        }
    }
    printf("…………\n");
    printf("总计： %d 行，加载 %d 条指令\n",*line_count,*inst_count);
    fclose(f);
    free(address_used); // 释放地址标记数组
}