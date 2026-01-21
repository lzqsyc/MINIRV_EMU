#include "../head/head.h"

int objdump_check(const char *line, uint32_t *line_addr, uint32_t instructions[8],int *count){
/*********************** 根据Instructions index 获取基准地址*******************************/
    char *colon = strchr(line,':');
    if (colon==NULL) return 0;
    // 提取Instruction index 指令索引
    char temp_addr[32]={0};
    strncpy(temp_addr,line,colon-line);
    temp_addr[colon-line] = '\0';
    char *endptr = NULL;  
    uint32_t Instr_index = (uint32_t)strtoul(temp_addr,&endptr,16);    // 数字字符串转16进制
    if (endptr==temp_addr) return 0;
    *line_addr = Instr_index * 4 ;     

/*********************************** 指令切片 ********************************************/
    const char *data_start = colon +1;
    char data_copy[512]={0};
    strncpy(data_copy,data_start,sizeof(data_copy)-1);
    data_copy[sizeof(data_copy)-1]= '\0';
 
    // token每次指向分隔符的切片首地址，并将切片指令字符串转化为16进制数据存入临时指令数组
    char *token = strtok(data_copy," \n\t\r");  
    while (token !=NULL  && *count <8){
        instructions[*count]=(uint32_t)strtoul(token,&endptr,16);
        if (endptr==token && *endptr != '\n'){
            return 0;
        }
        (*count)++;
        // 继续切片，参数NULL 继续切片操作，从上一次停止的位置开始，没有下一个token后返回NULL
        token = strtok(NULL," \t\n\r");
    }
    return 1;
}

// 检查第 idx 个指令槽是否被占用
int is_used(uint32_t *map, int idx){
    // idx / 32 -> 定位到数组的第几个整数
    // idx % 32 -> 定位到该整数内的第几位
    return map[idx / 32] & (1U << (idx % 32));
}

// 标记第 idx 个指令槽为“已占用”
void set_used(uint32_t *map, int idx){
    map[idx / 32] |= (1U << (idx % 32));
}

void load_objdump(const char *filepath,CPU_state *cpu,int *line_count,int *inst_count,int *error_count){
    char line[512];
  /*  方案1
    address_used 数组的每一个格子（element），代表的是 CPU 内存中一个 32 位（4字节）的坑位是否被占用。
    int *address_used = ( int *)calloc(instructions_max,sizeof(int));
    使用 int8_t (1字节) 代替 int (4字节) 
    uint8_t *address_used = (uint8_t *)calloc(instructions_max, sizeof(uint8_t));
      方案2
    int instructions_max = cpu->mem_size / 4;      最大总指令数，则需要监控做这么多位数
    int bitmap_size = (instructions_max + 31) /32;  每个int能存32位
    int32_t *address_used = (uint32_t*)calloc (bitmap_size,sizeof(uint32_t));
    设置两个函数，一个是验证是否写入，一个是写入了之后对bitmap_size 进行置位处理表示已经写入
  */
    // 1. 计算内存中有多少个“指令坑位” (假定每条指令4字节)
    int instructions_max = cpu->mem_size / 4;
    // 2. 计算位图大小：我们需要多少个 uint32_t 才能存下这么多 bit？
    // (instructions_max + 31) / 32 是为了向上取整，确保最后不满32个的也能存下
    int bitmap_size = (instructions_max + 31) / 32;
    // 3. 分配并清零内存
    uint32_t *address_used = (uint32_t *)calloc(bitmap_size, sizeof(uint32_t));
    if (address_used == NULL) return;

    FILE *f = fopen(filepath, "r");
    if (f==NULL) return ;
    // 循环读取文件每行
    while (fgets(line,sizeof(line),f))
    {
        (*line_count)++;
        int count = 0;
        uint32_t line_addr = 0;             // 行基准起始地址
        uint32_t instructions[8]={0};       // 行指令数临时存放数组
        clean_format(line);                 // 加载时的格式清理
        if (objdump_check(line,&line_addr,instructions,&count)){
        // 越界检查
        // 解释第八个指令，数组下标=7，第8个指令的起始地址。
            uint32_t last_addr = line_addr + (count-1)*4;  
         // 同理解释，四个字节地址，起始地址已经占据一个字节内存，所以+3     
            if (last_addr + 3 >= cpu->mem_size)                
            {
                if (*error_count < 5 )
                {
                    printf("警告：第%d行 - 地址 0x%08x 超出内存范围\n",*line_count,line_addr);
                }
                (*error_count)++;
                continue;
            }
        // 冲突检查
            int any_used = 0;     // 0=安全，1=冲突
            int used_index = -1;  // 中途指令索引
            for (int i = 0; i < count; i++){
                uint32_t inst_addr = line_addr + i*4;
                int idx = inst_addr / 4;           // 还原第几个指令的索引号
                if (is_used(address_used,idx)){    // 利用位验证是否已经写入
                    any_used = 1;
                    if (used_index == -1){
                        used_index = i;
                    }
                    break;
                }
            }
            if (any_used==1){
                if (*error_count < 5){
                    printf("警告: 第%d行 - 地址 0x%08x 已被使用，跳过整行\n", *line_count, line_addr);
                }
                (*error_count)++;
                continue;
            }
        // 写入内存与标记
            for (int i = 0; i < count; i++){
                uint32_t inst_addr = line_addr + i*4;
                mem_write(cpu,inst_addr+LOAD_START,instructions[i]);
                int idx = inst_addr / 4;
                set_used(address_used,idx);         // 写入第idx个指令的站位标记
                if ((*inst_count) < 8 ){
                    printf("    [0x%08x] = 0x%08x\n",inst_addr,instructions[i]);
                } else if((*inst_count) == 8){
                    printf("    ……后续指令加载信息省略\n");
                }
                (*inst_count)++;
            }
        }
    // 收尾
        if ((*line_count) % 1000 ==0){
        printf("已处理 %d 行，加载 %d 条指令\n", *line_count, *inst_count);
        }
    }
    printf("…………\n");
    printf("总计： %d 行，加载 %d 条指令\n",*line_count,*inst_count);
    fclose(f);
    free(address_used);
}