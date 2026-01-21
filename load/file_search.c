#include "../head/head.h"
//==================== 文件查找成功并且验证格式通过返回1，否则返回失败-1 ====================================
int file_search(CPU_state *cpu, char *filename){

/*******************文件路径查询并检查是否能打开*******************/
    if (filename == NULL || strlen(filename) == 0)
    {
        printf("输入文件名不能为空！\n");
        return -1;
    }
    char filepath[256] = {0};
    if (! hex_search(filename,filepath,sizeof(filepath)))
    {
        printf("错误：无法找到hex文件 %s \n",filepath);
        return -1;
    }
    printf("找到文件：%s\n",filepath);
    cpu_init(cpu);
/******************文件hex格式查询：objdump/simple,并加载***************************/
    int file_format=hex_format_check(filepath);
    int line_count = 0;
    int inst_count = 0;      
    int error_count = 0;
    switch (file_format){
        case 1:
            load_objdump(filepath,cpu,&line_count,&inst_count,&error_count);
            break;
        case 2:
            load_simple(filepath,cpu,&line_count,&inst_count,&error_count);
            break;
        default:
            return -1;
            break;
    }
    // 加载完成，显示加载结果
    printf("\n=== 加载结果 ===\n");
    printf("文件格式：%s\n",file_format == 1 ? "objdump格式" : "简单格式");
    printf("成功加载: %d 条指令\n", inst_count);
    printf("解析错误: %d 行\n", error_count);
    printf("总处理行数: %d\n", line_count);

    if (inst_count == 0)
    {
        printf("错误：没有成功加载任何指令\n");
    }
    if (error_count >0)
    {
        printf("警告：文件中有 %d 行解析错误/地址冲突\n",error_count);

    }
    return (error_count == 0) ? 1 : -1 ;
}