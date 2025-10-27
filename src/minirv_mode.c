#include "../head/head.h"
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

