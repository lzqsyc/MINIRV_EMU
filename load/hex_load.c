#include "../head/head.h"
void hex_load(CPU_state *cpu){
    char filename [256]={0};
    printf("\n ===Hex文件加载模式 ===\n");
    printf("支持的文件格式：\n");
    printf("1. 简单格式：每行32位十六进制指令\n");
    printf("2. objdump格式：地址：指令1 指令2 …… 指令8\n");
    printf("3. 自动在预设路径中寻找对应文件\n");
    printf("请输入hex文件名(输入q/Q 返回主菜单): ");

    if (scanf("%s",filename) !=1)
    {
        printf("输入指令错误！\n");
        return;
    }
    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF);    //赋值低于比较运算法

    if (strcmp(filename,"q") ==0 || strcmp(filename,"Q") == 0)
    {
        printf("返回主菜单\n");
        return;
    }
    cpu_init(cpu);

    int result = hex_execute(cpu,filename);
    if (result >=0 )
    {
        printf("文件加载成功！\n");
        int execute_mode = 0;
        do
        {
            printf("\n ===Hex文件执行模式 ===\n");
            printf("1. 调试模式(单步执行)\n");
            printf("2. 连续运行模式\n");
            printf("3. 退出当前模式回到主菜单\n");
            printf("请输入模式： ");
            if (scanf("%d",&execute_mode) !=1)
            {
                printf("输入错误，请重新输入选择！\n");
                 while ((c = getchar()) != '\n' && c != EOF);
            }
             while ((c = getchar()) != '\n' && c != EOF);
            switch (execute_mode)
            {
            case 1:
                cpu->cpu_running = 1;
                debug_mode(cpu);
                break;
            case 2:
                cpu->cpu_running = 1;
                run_mode(cpu);
                break;
            case 3:
                printf("退出当前模式，返回主菜单\n");
                break;
            default:
                printf("输入无效，请重新输入！\n");
                break;
            }
        } while (execute_mode !=3);
    }
    else{
        printf("文件加载失败！请检查文件格式或名称\n");
        char retry[5] = {0};
        printf("\n是否重新加载文件(yes/任意字符退出)?\n");
        if (scanf("%s",retry) == 1 && strcmp(retry,"yes") == 0)
        {
            while ((c = getchar()) != '\n' && c != EOF);
            hex_load(cpu);                                            // 递归调用加载程序
        }
        else{
            while ((c = getchar()) != '\n' && c != EOF);
            printf("返回主菜单！\n");
        }
        
    } 

}