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
    0x000012b7, // lui t0, 0x1         -> t0 = 0x00001000
    0x03428293, // addi t0, t0, 52     -> t0 = 0x00001034 (下一条 ebreak 地址)
    0x000280e7, // jalr ra, t0, 0      -> 跳到 ebreak

    // ===== 测试8: EBREAK =====
    0x00100073  // ebreak
};



// 测试程序加载
void test_load(CPU_state *cpu){
    printf("=== 加载完整测试程序 ===\n");
    uint32_t load_addr = TEST_START;
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

// 调试模式

void debug_mode(CPU_state *cpu){

    printf("\n=== 进入单步调试模式 ===\n");
    printf("命令: s(单步) r(运行) d(寄存器) m(内存) q(退出)\n");
    char command[100];
    int step_count = 0;
    cpu->cpu_running = 1;
    while (cpu->cpu_running)
    {
        printf("\n[Inst=%d,pc=0x%08x] > : ",step_count,cpu->pc);

        if (fgets(command,sizeof(command),stdin)==NULL){
            break;
        } else{
            command[strcspn(command,"\n")] = '\0';
        }
        if (strcmp(command, "s") == 0){
            printf("单步执行模式\n");
            if (cpu_core(cpu) !=0 ){
                printf("执行错误！\n");
                break;
            }
            step_count++;
            dump_gpr(cpu);
            if (!cpu->cpu_running){
                printf("遇到 ebreak 指令，程序终止\n");
                dump_gpr(cpu);  // 显示最终状态
                break;
            }
        } else if (strcmp(command,"r")==0){
            printf("连续执行模式\n");
            run_mode(cpu);
        } else if (strcmp(command,"d")==0)
        {
            dump_gpr(cpu);
        } else if (strncmp(command,"m ",2)==0)        // 从数组其实地址开始读取num个前缀字符
        {
            uint32_t addr =0;
            if (sscanf(command+2,"%x",&addr)==1)      // sscanf(addr*,"mode",*addr)第一个是读取地址，第二个写入地址
            {
                if (addr <cpu->mem_size-3)
                {
                    uint32_t value = mem_read(cpu,addr);
                    printf("内存[0x%08x] = 0x%08x\n",addr,value);
                }else {
                    printf("输入地址读取越界\n");
                }
            }else{
                printf("内存读取输入格式：m <16进制地址>\n");
            }
        } else if (strcmp(command,"q")==0)
        {
            printf("退出调试模式\n");
            break;
        }else if (strlen(command)>0){
            printf("未知命令。可用命令：s, r, d, m <addr>, q\n");
            continue;
        }  
    }
    cpu->cpu_running = 0;
    printf("已退出调试模式\n\n");
}

void run_mode(CPU_state *cpu){
    printf("\n=== 连续运行模式 ===\n");
    cpu->cpu_running = 1;
    while (cpu->cpu_running )
    {
        printf("\n--- 指令 %d ---\n", cpu->inst_count+1);
            // 检查PC是否越界
        if (cpu->pc >= cpu->mem_size) {
            printf("PC越界，停止运行\n");
            break;
        }
            // cpu执行程序，函数返回值不为0，跳出执行语句块，继续下一步
        if (cpu_core(cpu) !=0 ){
            printf("执行错误！停止运行\n");
            break;
        }
        // 检查是否遇到停止指令
        if (!cpu->cpu_running) { // ebreak
            printf("遇到ebreak指令，程序结束\n");
            break;
        }
    }
    printf("\n运行结束\n");
    printf("总执行指令数: %d\n", cpu->inst_count);
    printf("最终PC: 0x%08x\n", cpu->pc);
    printf("寄存器最终状态：\n");
    dump_gpr(cpu); 

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
