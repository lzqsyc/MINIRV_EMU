#include "../head/head.h"
//=============================== 预设路径中寻找文件名，找到文件返回0，失败返回非0 ======================
int hex_search(const char *filename, char *filepath,size_t path_size){
    const char *possible_paths[]={
        filename,
        "load/%s",
        "other/%s",
        "../hex/%s",
        "test/%s",
        "../test/%s",
        NULL
    };
// %s 格式说明符=占位符，利用snprintf来实现字符串替换（格式化拼接）
// snprintf 按指定格式将数据打印到字符串缓冲区地址去
    printf("\n正在搜索文件: %s\n", filename);
    printf("搜索路径:\n");
    for ( int i = 0; possible_paths[i]!= NULL; i++)
    {
        // 如果路径模板里包含 '%'，说明它是一个需要拼接的模板
        if (strchr(possible_paths[i],'%') != NULL){
        // 将fimename数据按possible_path[i]填入占位符后写入filepath目标路径，最多写入path_size大小数据
            snprintf(filepath,path_size,possible_paths[i],filename);
        }else{
         // 如果没有 '%'（比如数组的第一个元素 filename 本身），说明它是纯文本
            strncpy(filepath,possible_paths[i],path_size);
        }
        filepath[path_size - 1] = '\0'; // 确保字符串终止
        printf("  尝试: %s -> ", possible_paths[i]);
        printf("%s\n", filepath);
        FILE *f = fopen(filepath,"r");
        if (f!=NULL)
        {
            printf("文件：%s 打开成功!\n",filepath);
            fclose(f);
            return 1;
        }
    }
    // 所有预设路径都没有找到文件，并打开成功，则返回寻找文件失败
    printf("所有路径尝试失败\n");
    return 0;
    
}
// ==================================== 文件格式检查 ============================================
int hex_format_check(const char *filepath){
    FILE *f = fopen(filepath,"r");
    char line[512] = {0};
    int line_count = 0;
    int addr_count = MEM_SIZE / 4;
    // 内存空间被划分成以4字节为内存块连续的数组空间，addr_use[i]空间大小都是4byte, addr_use[i]返回值为0表示未使用该内存空间
    int *addr_use = (int *)calloc(addr_count,sizeof(int));
    if (addr_use == NULL)
    {
        fclose (f);
        printf("错误：无法分配地址标记数组\n");
        return 0;
    }

    printf("\n开始加载hex文件\n");
    printf("检查文件格式\n");
    //循环读取文件每行数据，进行解析处理
    while (fgets(line,sizeof(line),f))
    {
        line_count++;
        clean_format(line);
        uint32_t addr;
        uint32_t instructions[8];   // 格式检查时清理
        int count;

        if (objdump_check(line, &addr,instructions,&count))
        {
            if (line_count == 10)
            {
                printf("检查到objdump格式\n");
                fseek (f,0,SEEK_SET);
                return 1;
                break;
            }
        }
        else if (simple_check(line,instructions))
        {
            if (line_count == 10)
            {
                printf("检查到simple格式（地址已转化为4字节地址）\n");
                fseek (f,0,SEEK_SET);
                return 2;
                break;
            }
        }
        else{ 
            printf("未知文件格式，请重新检查加载！");
            return 0;
        }
    }
    fclose(f);
    free(addr_use);
    return 0;
}

// ================================ 去掉每行换行符，注释符，去掉首尾空白符 ===================================
void clean_format(char *line){
    if (line == NULL) return;
    // strcspn 返回字符串中第一个匹配到指定字符的下标
    line[strcspn(line,"\r")] = 0;
    line[strcspn(line,"\n")] = 0;
    // 去掉#后的注释，利用'\0'截断
    char *comment = strchr(line,'#');
    if (comment!=NULL)
    {
        *comment = 0;
    }
    // 去掉前导空白符
    char *start = line;
    while(isspace((unsigned char)*start)) start++;
    if (start!=line){
        memmove(line,start,strlen(start)+1);
    }
    // 去掉尾部空白符
    char *end = line+strlen(line)-1;
    if (end == NULL) return;
    while(isspace((unsigned char)*end)) {
        *end= '\0';
        end--;
    }
}

