#include "../head/head.h"

// 32位读取
uint32_t mem_read(CPU_state *cpu,uint32_t addr){
    if ( addr % 4 != 0)
    {
        printf("字输入读取地址对齐失败！ pc=0x%08x\n",addr);
        return 0;
    } else if(addr+3 >= cpu->mem_size){
        printf("字内存读取地址越界！pc=0x%08x\n",addr);
        return 0;
    }else {
        return *(uint32_t*)(cpu->memory+addr);      //自动从addr开始连续读取4个字节转化为32位输出
    }
}
// 32位写入：1、地址对齐；2、地址不能越界
void mem_write(CPU_state *cpu,uint32_t addr, uint32_t value){
    if ( addr % 4 != 0)
    {
        printf("字输入读写地址对齐失败！ pc=0x%08x\n",addr);
        return;
    } else if(addr+3 >= cpu->mem_size){
        printf("字内存写入地址越界！pc=0x%08x\n",addr);
        return;
    }
    // 强制转化memory字节指针为32位字指针并解引用，用于当前地址值的写入
    *(uint32_t*)(cpu->memory+addr) = value;
}
// 字节读取
uint8_t byte_read(CPU_state *cpu,uint32_t addr){
    if(addr >= cpu->mem_size){
        printf("字节内存读取地址越界！pc=0x%08x\n",addr);
        return 0;
    }else return *(uint8_t *)(cpu->memory+addr);
    //   return cpu->memory[addr];          //字节指针寻址读取：
}

// 字节写入
void byte_write(CPU_state *cpu,uint32_t addr,uint8_t value){
    if(addr >= cpu->mem_size){
        printf("字节内存写入地址越界！pc=0x%08x\n",addr);
        return;
    }
    else *(uint8_t*)(cpu->memory+addr) = value; 
     //cpu->memory[addr] = value;
}
