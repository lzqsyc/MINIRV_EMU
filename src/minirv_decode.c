#include "../head/head.h"
DecodeInst decode_inst(uint32_t cpu_inst){
    DecodeInst inst;
    inst.opcode = cpu_inst & 0x7f;
    inst.rd = (cpu_inst >> 7) & 0x1f;
    inst.funct3 = (cpu_inst >> 12) & 0x07;
    inst.rs1 = (cpu_inst >> 15) & 0x1f;
    inst.rs2 = (cpu_inst >> 20) & 0x1f;
    inst.funct7 = (cpu_inst >> 25) & 0x7f;
    inst.funct12 = (cpu_inst >> 20) & 0xfff;
    inst.imm = 0;
    inst.is_branch = 0;
    inst.is_jump = 0;
    // 立即数扩展
    switch (inst.opcode)
    {
    case OPCODE_LOAD:                                       // lw lbu
        inst.imm = (int32_t)(cpu_inst & 0xfff00000) >> 20;
        break;
    case OPCODE_STORE:                                      // sw sb
        inst.imm = (int32_t)((((cpu_inst & 0xfe000000) >> 20) | ((cpu_inst >> 7) & 0x1f)) << 20) >>20;
        break;
    case OPCODE_ADDI :                                      // addi
        inst.imm = (int32_t)(cpu_inst & 0xfff00000) >> 20;
        break;
    case OPCODE_ADD:                                        // add
        inst.imm = 0;
        break;
    case OPCODE_JALR:                                       // jalr
        inst.imm = (int32_t)(cpu_inst & 0xfff00000) >> 20; 
        inst.is_jump = 1;
        break;
    case OPCODE_LUI :                                       // lui
        inst.imm = cpu_inst & 0xfffff000;
        break;
    default:
        break;
        inst.imm = 0;
    }
    return inst;
}