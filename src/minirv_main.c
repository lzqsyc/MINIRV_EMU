#include "../head/head.h"
int main(){
    printf("=== MiniRV RISC-V 模拟器启动 ===\n");
    CPU_state cpu;
    int mode =0;
    do
    {
        printf("\n=== 主菜单 ===\n");
        printf("1. 指令功能测试模式\n");
        printf("2. hex文件加载模式\n"); 
        printf("3. 退出模拟器！\n"); 
        printf("请输入您的模式选择：");

        if (scanf("%d",&mode) !=1)
        {
            printf("输入错误！");
            clear_input_buffer();
            continue;   // 重复循环语句，回到循环开始并非终止
        }
        clear_input_buffer();
        cpu.pc = LOAD_START;
        switch (mode){
            case 1:
                inst_test(&cpu);
                break;
            case 2:
                cpu_init(&cpu);
                hex_load(&cpu);
                cpu_free(&cpu);
                break;
            case 3:
                printf("退出模拟器\n");
                break;
            default:
                printf("无效选择，请重新输入\n");
                break;
        }
    } while (mode != 3);  
    // 显示最终状态
    printf("\n=== MiniRV 模拟器关闭 ===\n");
    return 0;

}