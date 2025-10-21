#include "../head/head.h"

void hex_load(CPU_state *cpu) {
    char filename[256];
    
    printf("\n=== Hex文件加载模式 ===\n");
    printf("支持的文件格式:\n");
    printf("  1. 简单格式: 每行一个32位十六进制指令\n");
    printf("  2. Objdump格式: '地址: 数据1 数据2 ... 数据8'\n");
    printf("    注意: objdump地址会自动乘以4转换为字节地址\n");
    printf("  3. 自动在load/, hex/, test/目录中查找文件\n");
    printf("\n请输入hex文件名 (输入'q/Q'返回主菜单): ");
    
    if (scanf("%255s", filename) != 1) {
        printf("输入错误\n");
        return;
    }
    
    // 清空输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // 检查是否要退出
    if (strcmp(filename, "q") == 0 || strcmp(filename, "Q") == 0) {
        printf("返回主菜单\n");
        return;
    }
    
    // 初始化CPU（确保内存已分配）
    cpu_init(cpu);
    
    // 加载文件
    int result = load_hex_file(cpu, filename);
    
    if (result >= 0) {
        printf("\n文件加载成功！\n");
        
        // 选择运行模式
        int mode_choice;
        do {
            printf("\n选择运行模式:\n");
            printf("1. 调试模式 (单步执行)\n");
            printf("2. 连续运行模式\n");
            printf("3. 退出当前模式 (返回主菜单)\n");
            printf("请选择: ");
            
            if (scanf("%d", &mode_choice) != 1) {
                printf("输入错误，请重新选择\n");
                // 清空输入缓冲区
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            
            // 清空输入缓冲区
            while ((c = getchar()) != '\n' && c != EOF);
            
            switch (mode_choice) {
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
                    printf("无效选择，请重新选择\n");
                    continue;
            }
        } while (mode_choice != 1 && mode_choice != 2 && mode_choice != 3);
    } else {
        printf("文件加载失败，请检查文件名和格式\n");
        
        // 询问是否重试
        char retry;
        printf("\n是否重新尝试加载文件? (y/n): ");
        if (scanf(" %c", &retry) == 1 && (retry == 'y' || retry == 'Y')) {
            // 清空输入缓冲区
            while ((c = getchar()) != '\n' && c != EOF);
            hex_load(cpu); // 递归调用自身
        } else {
            // 清空输入缓冲区
            while ((c = getchar()) != '\n' && c != EOF);
            printf("返回主菜单\n");
        }
    }
}