#include "../head/head.h"

uint32_t test_instruction[] = {
    // ===== 测试1: LUI =====
    0x00004537, // lui a0, 0x4        -> a0 = 0x00004000

    // ===== 测试2: ADDI =====
    0x00150513, // addi a0, a0, 1     -> a0 = 0x00004001

    // ===== 测试3: ADD =====
    0x00200593, // addi a1, zero, 2   -> a1 = 0x2
    0x00b50633, // add a2, a0, a1     -> a2 = 0x00004003

    // ===== 测试4: SW =====
    0x20000693, // addi a3, zero, 0x200 -> a3 = 0x200
    0x00c6a023, // sw a2, 0(a3)       -> mem[0x200] = 0x00004003

    // ===== 测试5: LW =====
    0x0006a703, // lw a4, 0(a3)       -> a4 = mem[0x200]

    // ===== 测试6: SB + LBU =====
    0x0ff00793, // addi a5, zero, 0xff -> a5 = 0xff
    0x00f68023, // sb a5, 0(a3)       -> mem[0x200] = 0xff
    0x0006c783, // lbu a5, 0(a3)      -> a5 = 0xff

    // ===== 测试7: JALR 跳到下一条 EBREAK =====
    0x000002b7, // lui t0, 0x1         -> t0 = 0x0000000
    0x03428293, // addi t0, t0, 52     -> t0 = 0x0000034 (下一条 ebreak 地址)
    0x000280e7, // jalr ra, t0, 0      -> 跳到 ebreak

    // ===== 测试8: EBREAK =====
    0x00100073  // ebreak
};



// 测试程序加载
void test_load(CPU_state *cpu){
    printf("=== 加载完整测试程序 ===\n");
    uint32_t load_addr = LOAD_START;
    int inst_count = sizeof(test_instruction)/sizeof(test_instruction[0]);
    printf("程序大小: %d 条指令\n", inst_count);
    printf("加载地址: 0x%08x\n", load_addr);
    
    for (int i = 0; i < inst_count; i++)
    {
        mem_write(cpu, load_addr + i * 4, test_instruction[i]);
        printf("    [0x%08x] = 0x%08x\n", load_addr + i * 4, test_instruction[i]);
    }
    
    // 设置PC到程序开始
    cpu->pc = load_addr;
    printf("PC设置为: 0x%08x\n", cpu->pc);
}

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
        clear_input_buffer();
        continue;   // 重复循环语句，回到循环开始并非终止
    }
    clear_input_buffer();
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

void show_help() {
    printf("=== MiniRV 模拟器使用说明 ===\n");
    printf("模式选择:\n");
    printf("  1 - 单步调试模式 (逐步执行，可查看状态)\n");
    printf("  2 - 连续运行模式 (自动执行完整程序)\n");
    printf("  3 - 验证测试结果 (显示测试通过情况)\n");
    printf("  4 - 显示寄存器状态\n");
    printf("  5 - 退出模拟器\n");
    printf("\n测试程序说明:\n");
    printf("  该程序测试8条RISC-V指令:\n");
    printf("  - LUI:    加载高位立即数\n");
    printf("  - ADDI:   立即数加法\n");
    printf("  - ADD:    寄存器加法\n");
    printf("  - SW:     存储字到内存\n");
    printf("  - LW:     从内存加载字\n");
    printf("  - SB:     存储字节到内存\n");
    printf("  - LBU:    无符号字节加载\n");
    printf("  - JALR:   跳转并链接寄存器\n");
}
