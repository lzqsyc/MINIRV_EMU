#include "../head/head.h"



int load_hex_file(CPU_state *cpu, const char *filename) {
    if (filename == NULL || strlen(filename) == 0) {
        printf("错误: 文件名不能为空\n");
        return -1;
    }
    
    // 查找文件
    char filepath[256];
    if (!find_hex_file(filename, filepath, sizeof(filepath))) {
        printf("错误: 无法找到hex文件 '%s'\n", filename);
        printf("搜索路径包括:\n");
        printf("  - %s\n", filename);
        printf("  - load/%s\n", filename);
        printf("  - hex/%s\n", filename);
        printf("  - test/%s\n", filename);
        return -1;
    }
    
    printf("找到文件: %s\n", filepath);
    
    FILE *f = fopen(filepath, "r");
    if (!f) {
        printf("错误: 无法打开文件 '%s'\n", filepath);
        perror("具体错误");
        return -1;
    }
    
    char line[512];
    int line_num = 0;
    int inst_count = 0;
    int error_count = 0;
    int objdump_format = 0;
    
    // 创建地址使用标记数组
    int address_count = cpu->mem_size / 4;
    int *address_used = (int *)calloc(address_count, sizeof(int));
    if (address_used == NULL) {
        printf("错误: 无法分配地址标记数组\n");
        fclose(f);
        return -1;
    }
    
    printf("开始加载hex文件...\n");
    printf("检测文件格式...\n");
    
    // 首先检测文件格式
    while (fgets(line, sizeof(line), f)) {
        line_num++;
        clean_line(line);
        
        if (line[0] == '\0') continue;
        
        // 检查是否是objdump格式
        uint32_t address;
        uint32_t instructions[8];
        int count;
        
        if (parse_objdump_line(line, &address, instructions, &count)) {
            objdump_format = 1;
            printf("检测到objdump格式（地址已乘以4转换为字节地址）\n");
            break;
        }
        if (line_num > 10) {
            printf("警告: 无法确定文件格式，默认使用简单格式\n");
            break;
        }
    }
    
    // 重置文件指针
    fseek(f, 0, SEEK_SET);
    line_num = 0;
    
    // 根据检测到的格式进行加载
    uint32_t current_addr = LOAD_START;
    
    while (fgets(line, sizeof(line), f)) {
        line_num++;
        clean_line(line);
        
        if (line[0] == '\0') continue;
        
        if (objdump_format) {
            // 解析objdump格式
            uint32_t address;
            uint32_t instructions[8];
            int count;
            
            if (parse_objdump_line(line, &address, instructions, &count)) {
                // 检查整行数据的地址范围
                uint32_t last_addr = address + (count - 1) * 4;
                if (last_addr + 3 >= cpu->mem_size) {
                    // 只显示前几个超出范围的警告
                    if (error_count < 5) {
                        printf("警告: 第%d行 - 地址 0x%08x 超出内存范围\n", line_num, address);
                    }
                    error_count++;
                    continue;
                }
                
                // 检查这一行是否有任何地址已被使用
                int any_used = 0;
                int first_used_index = -1;
                for (int i = 0; i < count; i++) {
                    uint32_t inst_addr = address + i * 4;
                    if (address_used[inst_addr/4]) {
                        any_used = 1;
                        if (first_used_index == -1) {
                            first_used_index = i;
                        }
                        break;
                    }
                }
                
                // 如果整行地址都已被使用，跳过这一行
                if (any_used) {
                    // 只显示前几个重复地址警告
                    if (error_count < 5) {
                        printf("警告: 第%d行 - 地址 0x%08x 已被使用，跳过整行\n", line_num, address);
                    }
                    error_count++;
                    continue;
                }
                
                // 加载整行数据
                for (int i = 0; i < count; i++) {
                    uint32_t inst_addr = address + i * 4;
                    
                    mem_write(cpu, inst_addr, instructions[i]);
                    address_used[inst_addr/4] = 1; // 标记为已使用
                    
                    // 只显示前10条指令的详细信息
                    if (inst_count < 10) {
                        printf("  [0x%08x] = 0x%08x\n", inst_addr, instructions[i]);
                    } else if (inst_count == 10) {
                        printf("  ... (后续指令加载信息省略)\n");
                    }
                    inst_count++;
                }
                
                // 更新当前地址（用于简单格式）
                current_addr = address + count * 4;
            } else {
                // 只显示前几个解析错误
                if (error_count < 5) {
                    printf("警告: 第%d行解析失败: '%s'\n", line_num, line);
                }
                error_count++;
            }
        } else {
            // 解析简单格式
            uint32_t instruction;
            if (parse_simple_hex_line(line, &instruction)) {
                if (current_addr + 3 >= cpu->mem_size) {
                    printf("警告: 第%d行 - 地址 0x%08x 超出内存范围，停止加载\n", line_num, current_addr);
                    break;
                }
                
                // 检查地址是否已被使用
                if (address_used[current_addr/4]) {
                    printf("警告: 第%d行 - 地址 0x%08x 已被使用，跳过写入\n", line_num, current_addr);
                    error_count++;
                    current_addr += 4;
                    continue;
                }
                
                mem_write(cpu, current_addr, instruction);
                address_used[current_addr/4] = 1; // 标记为已使用
                
                if (inst_count < 10) {
                    printf("  [0x%08x] = 0x%08x\n", current_addr, instruction);
                } else if (inst_count == 10) {
                    printf("  ... (后续指令加载信息省略)\n");
                }
                
                current_addr += 4;
                inst_count++;
            } else {
                // 只显示前几个解析错误
                if (error_count < 5) {
                    printf("警告: 第%d行解析失败: '%s'\n", line_num, line);
                }
                error_count++;
            }
        }
        
        // 显示进度（每1000行）
        if (line_num % 1000 == 0) {
            printf("已处理 %d 行，加载 %d 条指令\n", line_num, inst_count);
        }
    }
    
    fclose(f);
    free(address_used); // 释放地址标记数组
    
    printf("\n=== 加载结果 ===\n");
    printf("文件格式: %s\n", objdump_format ? "objdump格式" : "简单hex格式");
    if (objdump_format) {
        printf("说明: objdump地址已自动乘以4转换为字节地址\n");
    }
    printf("成功加载: %d 条指令\n", inst_count);
    printf("解析错误: %d 行\n", error_count);
    printf("总处理行数: %d\n", line_num);
    
    if (inst_count == 0) {
        printf("错误: 没有成功加载任何指令\n");
        return -1;
    }
    
    if (error_count > 0) {
        printf("警告: 文件中有 %d 行解析错误或地址冲突\n", error_count);
        if (error_count > 5) {
            printf("(只显示了前5个错误/警告)\n");
        }
    } else {
        printf("文件加载完全成功，无错误\n");
    }
    
    return (error_count == 0) ? 0 : 1;
}
