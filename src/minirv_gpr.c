#include "../head/head.h"

static const char* gpr_names[32] = {
    "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};
// gpr读取
uint32_t gpr_read(CPU_state *cpu, uint8_t reg){
    if (reg >= 32)
    {
        printf("输入读取寄存器无效！pc=0x%08x, reg=x%d\n",cpu->pc,reg);
        return 0;
    }else {
        return cpu->GPR[reg];
    }
}

// gpr写入
void gpr_write(CPU_state *cpu, uint8_t reg, uint32_t value){
    if (reg >= 32){
        printf("输入读取寄存器无效！pc=0x%08x, reg=x%d\n",cpu->pc,reg);
        return ;
    }
    else if (reg == 0){
        printf("寄存器x0不能写入！\n");
        return ;
    }
    else {
        cpu->GPR[reg] = value; 
        }
}

void dump_gpr(CPU_state *cpu){

    printf("\n寄存器状态 (PC=0x%08x)      \n", cpu->pc);
  
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("x%2d(%4s)=0x%08x%c",count,gpr_names[count],cpu->GPR[count],
                    j < 3 ?'\t':'\n');
            count++;
        }
         printf(" \n");  // 每行结束
    }
    printf("\n");
}