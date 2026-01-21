#include "../head/head.h"
// cpu状态初始化
void cpu_init(CPU_state *cpu){
    cpu->mem_size = MEM_SIZE;
    cpu->memory = (uint8_t*)malloc(cpu->mem_size); //  将内存大小转化为字节数组，并且由字节指针寻址
    if (cpu->memory == NULL)
    {
        printf("cpu模拟内存分配失败");
        exit(1);
    }
    memset(cpu->memory,0,cpu->mem_size);
    memset(cpu->GPR,0,sizeof(cpu->GPR));
    cpu->inst_count = 0;
    cpu->cycle_count = 0;
    cpu->cpu_running = 0;
}

// cpu堆上的动态内存释放
void cpu_free(CPU_state*cpu){
    if (cpu->memory != NULL)
    {
        free(cpu->memory);
        if (cpu->memory == NULL) 
        {
            printf("cpu执行模拟结束！内存已释放！");
        }
    }
} 

// 取值，译码，执行，访存，写回
int cpu_core(CPU_state *cpu){
    // fetch
    if (cpu->cpu_running == 0)
    {
        return -1;
    }
    uint32_t cpu_inst = mem_read(cpu,cpu->pc);
    if (cpu_inst >= 0xffffffff)
    {
        printf("指令越界！pc=0x%08x,inst=0x%08x\n",cpu->pc,cpu_inst);
        cpu->cpu_running = 0;
        return -1;

    }
    // 译码
    DecodeInst inst = decode_inst(cpu_inst);
    // 执行
    if (inst_execute(cpu, inst) != 0) {
        printf("指令执行失败\n");
        cpu->cpu_running = 0;
        return -1;
    }
    // pc更新
    if ( ! inst.is_jump )
    {
       cpu->pc = cpu->pc+4;
    }
    cpu->cycle_count++;
    cpu->inst_count++;
    return 0;
}
