#ifndef MINIRV_EMU
#define MINIRV_EMU

#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>

#define MEM_SIZE (512*1024)
#define LOAD_START 0x00000000
// 操作码
#define OPCODE_LOAD     0x03                // lw lbu
#define OPCODE_STORE    0x23                // sw sb
#define OPCODE_ADDI     0x13                // addi
#define OPCODE_ADD      0x33                // add
#define OPCODE_LUI      0x37                // lui
#define OPCODE_JALR     0x67                // jalr
#define OPCODE_EBREAK   0X73                // ebreak

// 功能码-funct3/funct3/funct12
#define FUNCT3_lw       0x2
#define FUNCT3_lbu      0x4
#define FUNCT3_sw       0x2
#define FUNCT3_sb       0x0
#define FUNCT3_addi     0x0
#define FUNCT3_add      0x0
#define FUNCT3_jalr     0x0
#define FUNCT12_ebreak  0x1

typedef struct 
{
    uint32_t pc;
    uint32_t GPR[32];
    uint32_t mem_size;
    uint8_t *memory;
    int cpu_running ;
    int inst_count;
    int cycle_count;
}CPU_state;

typedef struct 
{
    uint8_t opcode;
    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;
    uint8_t funct3;
    uint8_t funct7;
    uint16_t funct12;
    int32_t imm;
    int is_jump;
    int is_branch;
}DecodeInst;

// 内存读写
uint32_t mem_read(CPU_state *cpu,uint32_t addr);
void mem_write(CPU_state *cpu,uint32_t addr, uint32_t value);
uint8_t byte_read(CPU_state *cpu,uint32_t addr);
void byte_write(CPU_state *cpu,uint32_t addr,uint8_t value);
// 寄存器读写
uint32_t gpr_read(CPU_state *cpu, uint8_t reg);
void gpr_write(CPU_state *cpu, uint8_t reg, uint32_t value);
void dump_gpr(CPU_state *cpu);
// cpu执行步骤
void cpu_init(CPU_state *cpu);
void cpu_free(CPU_state*cpu);
int cpu_core(CPU_state *cpu);
DecodeInst decode_inst(uint32_t cpu_inst);
int inst_execute(CPU_state *cpu,DecodeInst inst);
// 指令测试
void test_load(CPU_state *cpu);
void debug_mode(CPU_state *cpu);
void run_mode(CPU_state *cpu);
void show_help();
void verify_test_results(CPU_state *cpu);
void inst_test(CPU_state *cpu);
// 指令执行
void lw(CPU_state *cpu,DecodeInst inst);
void lbu(CPU_state *cpu,DecodeInst inst);
void sw(CPU_state *cpu,DecodeInst inst);
void sb(CPU_state *cpu,DecodeInst inst);
void addi(CPU_state *cpu,DecodeInst inst);
void add(CPU_state *cpu,DecodeInst inst);
void lui(CPU_state *cpu,DecodeInst inst);
void jalr(CPU_state *cpu,DecodeInst inst);
void ebreak(CPU_state *cpu,DecodeInst inst);
//.hex文件加载         
/*                            
int find_hex_file(const char *filename, char *found_path, size_t path_size) ;     
int parse_objdump_line(const char *line, uint32_t *address, uint32_t instructions[8], int *count);
int parse_simple_hex_line(const char *line, uint32_t *instruction);
void clean_line(char *line) ;
int load_hex_file(CPU_state *cpu, const char *filename);
void file_load(CPU_state *cpu);
// file_load-2
*/ 
void clear_input_buffer();
void file_load(CPU_state *cpu,int *is_ok);
int hex_search(const char *filename, char *filepath,size_t path_size);
int hex_format_check(const char *filepath);
void clean_format(char *line);
int objdump_check(const char *line, uint32_t *addr, uint32_t instructions[8],int *count);
void load_objdump(const char *filepath,CPU_state *cpu,int *line_count,int *inst_count,int *error_count);
int simple_check(const char *line,uint32_t *instructions);
void load_simple(const char *line, CPU_state *cpu,int *line_count,int *inst_count,int *error_count);
int file_search(CPU_state *cpu, char *filename);

#endif