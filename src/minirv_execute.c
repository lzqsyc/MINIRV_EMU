#include "../head/head.h"

int inst_execute(CPU_state *cpu,DecodeInst inst){
    printf("当前pc=0x%08x, Inst=0x%08x\n",cpu->pc,mem_read(cpu,cpu->pc));
    switch (inst.opcode)
    {
    case OPCODE_LOAD: 
        switch (inst.funct3){
            case FUNCT3_lw:         // x[rd] = sext(M[x[rs1] + sext(offset)][31:0])
                lw(cpu,inst);
                break;
            case FUNCT3_lbu:        // x[rd] = M[x[rs1] + sext(offset)][7:0]
                lbu(cpu,inst);
                break;
            default:
                printf("未知执行指令！funct3=0x%02x",inst.funct3);
                return -1;
        }
        break;
    
    case OPCODE_STORE: 
        switch (inst.funct3){
            case FUNCT3_sw:             // M[x[rs1] + sext(offset) = x[rs2][31: 0]
                sw(cpu,inst);
                break;
            case FUNCT3_sb:             // M[x[rs1] + sext(offset) = x[rs2][7: 0]
                sb(cpu,inst);
                break;
            default:
                printf("未知执行指令！funct3=0x%02x",inst.funct3);
                return -1;
        }
        break;
        
    case OPCODE_ADDI:                   // x[rd] = x[rs1] + sext(immediate)
        if (inst.funct3 == FUNCT3_addi){
            addi(cpu,inst);
        }else {
            printf("未实现的算术指令: funct3=%d\n", inst.funct3);
            return -1;
        }
        break;
    
    case OPCODE_ADD:                   // x[rd] = x[rs1] + x[rs2]
        if (inst.funct3 == FUNCT3_add && inst.funct7 ==0){
            add(cpu,inst);
        }else {
            printf("未实现的算术指令: funct3=%d\n", inst.funct3);
            return -1;
        }
        break;
    
    case OPCODE_LUI:                    // x[rd] = sext(immediate[31:12] << 12)
        lui(cpu,inst);
        break;
    
    case OPCODE_JALR:                   // t =pc+4; pc=(x[rs1]+sext(offset))&~1; x[rd]=t
        if (FUNCT3_jalr == inst.funct3 ){
            jalr(cpu,inst);
        }   
        break;
    
    case OPCODE_EBREAK:                 // RaiseException(Breakpoint)
        ebreak(cpu,inst);
        break;
    default:
        printf("未知操作码: 0x%02x\n", inst.opcode);
        return -1;
    }

    return 0;
}

void lw(CPU_state *cpu,DecodeInst inst){
            uint32_t rs1_old = gpr_read(cpu,inst.rs1);
            uint32_t addr = (uint32_t)((int32_t) rs1_old +inst.imm);
            uint32_t value = mem_read(cpu,addr);
            gpr_write(cpu,inst.rd,value);
            printf("    lw: x%d = x%d(0x%08x) + %d = 0x%08x\n",inst.rd,inst.rs1,rs1_old,inst.imm,value);
            
}

void lbu(CPU_state *cpu,DecodeInst inst){
            uint32_t rs1_old = gpr_read(cpu,inst.rs1);
            uint32_t addr = (uint32_t)((int32_t) rs1_old +inst.imm);
            uint32_t value = byte_read(cpu,addr);       // 0扩展
            gpr_write(cpu,inst.rd,value);
            printf("    lbu: x%d = x%d(0x%08x) + %d = 0x%08x\n",inst.rd,inst.rs1,rs1_old,inst.imm,value);     
}

void sw(CPU_state *cpu,DecodeInst inst){
            uint32_t rs1_old = gpr_read(cpu,inst.rs1);
            uint32_t addr = (uint32_t)((int32_t)rs1_old + inst.imm);
            uint32_t value = gpr_read(cpu,inst.rs2);  
            mem_write(cpu,addr,value);
            printf("    sw: [x%d(0x%08x) + %d] = [0x%08x] = x%d(0x%08x)[31:0] = 0x%08x\n", 
                                    inst.rs1,rs1_old,inst.imm,addr,inst.rs2,value,value);       
}

void sb(CPU_state *cpu,DecodeInst inst){
            uint32_t rs2_old = gpr_read(cpu,inst.rs2);
            uint32_t rs1_old = gpr_read(cpu,inst.rs1);
            uint32_t addr = (uint32_t)((int32_t)rs1_old + inst.imm);
            uint8_t value = (uint8_t)(gpr_read(cpu,inst.rs2) & 0xff);
            byte_write(cpu,addr,value);
            printf("    sb: [x%d(0x%08x) + %d] = [0x%08x] = x%d(0x%08x)[7:0] = 0x%02x\n",
                                    inst.rs1,rs1_old,inst.imm,addr,inst.rs2,rs2_old,value);    
}

void addi(CPU_state *cpu,DecodeInst inst){
            uint32_t rs1_old = gpr_read(cpu,inst.rs1);
            uint32_t value = (uint32_t)((int32_t)rs1_old + inst.imm);
            gpr_write(cpu,inst.rd,value);
            printf("    addi: x%d = x%d(0x%08x) + %d = 0x%08x\n",inst.rd,inst.rs1,rs1_old,inst.imm,value);
}

void add(CPU_state *cpu,DecodeInst inst){
            uint32_t rs2_old = gpr_read(cpu,inst.rs2);
            uint32_t rs1_old = gpr_read(cpu,inst.rs1);
            uint32_t value = rs1_old+rs2_old;
            gpr_write(cpu,inst.rd,value);
            printf("    add: x%d = x%d(0x%08x) + x%d(0x%08x) = 0x%08x\n",inst.rd,inst.rs1,rs1_old,inst.rs2,rs2_old,value);
}

void lui(CPU_state *cpu,DecodeInst inst){
        gpr_write(cpu,inst.rd,inst.imm);
        printf("    lui: x%d = imm(0x%08x)\n",inst.rd,inst.imm);   
}

void jalr(CPU_state *cpu,DecodeInst inst){
            uint32_t pc_old = cpu->pc;
            uint32_t rs1_old = gpr_read(cpu,inst.rs1);
            uint32_t addr = (uint32_t)((int32_t)rs1_old + inst.imm);
            uint32_t t = pc_old +4 ;
            gpr_write(cpu,inst.rd,t);
            cpu->pc = addr & ~1 ;    // 清除最低位
            printf("    jalr: pc = ((x%d[0x%08x] + %d)&~1) = 0x%08x\n   x%d(0x%08x) = pc[0x%08x] + 4 = 0x%08x\n",
                                        inst.rs1,rs1_old,inst.imm,addr & ~1,inst.rd,cpu->GPR[inst.rd],pc_old,t);    
}

void ebreak(CPU_state *cpu,DecodeInst inst){
            if (FUNCT12_ebreak==inst.funct12)
            {
                cpu->cpu_running = 0;
                printf("\n=== 遇到 ebreak 指令，程序终止 ===\n");
            } 
}

