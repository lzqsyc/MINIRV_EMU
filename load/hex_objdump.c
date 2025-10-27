#include "../head/head.h"

int objdump_check(const char *line, uint32_t *addr, uint32_t instructions[8],int *count){
/*************找冒号→取地址→（×4）→切 token→转字节→按小端组 32 位→回填 instructions 和 *count***************/
    char *colon = strchr(line,':');
    if (colon==NULL) return 0;
    // 提取：前的字地址字符串
    char line_addr[32]={0};
    strncpy(line_addr,line,colon-line);
    line_addr[colon-line] = '\0';

    char *endptr = NULL;      //从line_addr开始转化到endptr结束，endptr保存的是转为到那个地址结束的
    uint32_t word_addr = (uint32_t)strtoul(line_addr,&endptr,16);
    if (endptr==line_addr) return 0;
    *addr = word_addr * 4 ;     

    const char *data_start = colon +1;
    char data_copy[512]={0};
    strncpy(data_copy,data_start,sizeof(data_copy)-1);
    data_copy[sizeof(data_copy)-1]= '\0';
    /*
    const char *data_start = colon + 1;
    char data_copy[512];
    size_t n = strnlen(data_start, sizeof(data_copy) - 1);
    memcpy(data_copy, data_start, n);
    data_copy[n] = '\0';
    */
    // token每次指向非分隔符的切片首地址
    char *token = strtok(data_copy," \n\t\r");  

    while (token !=NULL  && *count <8)
    {
        instructions[*count]=(uint32_t)strtoul(token,&endptr,16);
        if (endptr==token && *endptr != '\n')
        {
            return 0;
        }
        (*count)++;
        // 继续切片，参数NULL 继续切片操作，从上一次停止的位置开始，没有下一个token后返回NULL
        token = strtok(NULL," \t\n\r");
    }
    return 1;
}

void load_objdump(const char *filepath,CPU_state *cpu,int *line_count,int *inst_count,int *error_count){
    char line[512];
    int address_count = cpu->mem_size/4;
    int *address_used = ( int *)calloc(address_count,sizeof(int));
    if (address_used == NULL) return;

    FILE *f = fopen(filepath, "r");
    if (f==NULL) return ;
    while (fgets(line,sizeof(line),f))
    {
        (*line_count)++;
        int count = 0;
        uint32_t address = 0;
        uint32_t instructions[8]={0};
        clean_format(line);
        if (objdump_check(line,&address,instructions,&count)){
            uint32_t last_addr = address + (count-1)*4;
            if (last_addr + 3 >= cpu->mem_size)
            {
                if (*error_count < 5 )
                {
                    printf("警告：第%d行 - 地址 0x%08x 超出内存范围\n",*line_count,address);
                }
                (*error_count)++;
                continue;
            }
            int any_used = 0;
            int used_index = -1;
            for (int i = 0; i < count; i++){
                uint32_t inst_addr = address + i*4;
                if (address_used[inst_addr/4]){
                    any_used = 1;
                    if (used_index == -1){
                        used_index = i;
                    }
                    break;
                }
            }
            if (any_used==1){
                if (*error_count < 5){
                    printf("警告: 第%d行 - 地址 0x%08x 已被使用，跳过整行\n", *line_count, address);
                }
            (*error_count)++;
                continue;
            }
            for (int i = 0; i < count; i++){
                uint32_t inst_addr =address + i*4;
                mem_write(cpu,inst_addr+LOAD_START,instructions[i]);
                address_used[inst_addr/4] = 1;
                if ((*inst_count) < 8 ){
                    printf("    [0x%08x] = 0x%08x\n",inst_addr,instructions[i]);
                } else if((*inst_count) == 8){
                    printf("    ……后续指令加载信息省略\n");
                }
                (*inst_count)++;
            }
        }
        if ((*line_count) % 1000 ==0){
        printf("已处理 %d 行，加载 %d 条指令\n", *line_count, *inst_count);
        }
    }
    printf("…………\n");
    printf("总计： %d 行，加载 %d 条指令\n",*line_count,*inst_count);
    fclose(f);
    free(address_used);
}