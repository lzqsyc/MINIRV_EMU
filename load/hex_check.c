#include "../head/head.h"

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
    printf("\n正在搜索文件: %s\n", filename);
    printf("搜索路径:\n");
    for ( int i = 0; possible_paths[i]!= NULL; i++)
    {
        if (strchr(possible_paths[i],'%') != NULL)
        {
            snprintf(filepath,path_size,possible_paths[i],filename);
        }else{
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
        uint32_t instructions[8];
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

void clean_format(char *line){
    if (line == NULL) return;
    line[strcspn(line,"\r")] = 0;
    line[strcspn(line,"\n")] = 0;
    char *comment = strchr(line,'#');
    if (comment!=NULL)
    {
        *comment = 0;
    }
    char *start = line;
    while(isspace((unsigned char)*start)) start++;
    if (start!=line){
        memmove(line,start,strlen(start)+1);
    }

    char *end = line+sizeof(line)-1;
    if (end == NULL) return;
    while(isspace((unsigned char)*end)) {
        *(end+1) = '\0';
        end--;
    }
}

