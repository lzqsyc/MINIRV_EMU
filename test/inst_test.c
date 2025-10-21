#include "../head/head.h"

void inst_test(CPU_state *cpu){
int choice = 0;
    do
{
    printf("\n=== 主菜单 ===\n");
    printf("1. 单步调试模式\n");
    printf("2. 连续运行模式\n"); 
    printf("3. 验证测试结果\n");
    printf("4. 显示寄存器状态\n");
    printf("5. 显示帮助\n");
    printf("6. 退出\n");
    printf("请选择模式: ");

    if (scanf("%d",&choice) !=1)
    {
        printf("输入错误！");
        while (getchar() !='\n');
        continue;   // 重复循环语句，回到循环开始并非终止
    }
    while (getchar()!='\n');
    switch (choice)
    {
        case 1:
            cpu_init(cpu);
            test_load(cpu);
            debug_mode(cpu);
            break;
        case 2:
            cpu_init(cpu);
            test_load(cpu);
            run_mode(cpu);
            break;
        case 3:
            verify_test_results(cpu);
            break;
        case 4:
            dump_gpr(cpu);
            break;
        case 5:
            show_help();
            break;
        case 6:
            printf("指令功能测试模式\n");
            break;
        default:
            printf("无效选择，请重新输入\n");
    }
    } while ( choice !=6);              // ！= 6继续循环， == 6 退出
    // 显示最终状态
    printf("\n=== 最终状态 ===\n");
    dump_gpr(cpu);
    cpu_free(cpu);
}