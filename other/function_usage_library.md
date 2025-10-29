# Minirv_emu 函数使用文件库（标准库函数手册版）

本手册列出了项目中使用的所有 C 标准库函数，包括原型、返回值、参数意义、项目中用途、各文件使用次数、以及典型代码片段。

---

## atoi

**原型：** `int atoi(const char *nptr);`

**返回值：** 转换后的int。

**参数：**

- `nptr`：数字串

**项目用途：** （当前项目中未必出现，留作扩展。）

---

## calloc

**原型：** `void *calloc(size_t nmemb, size_t size);`

**返回值：** 成功返回指针，失败NULL。

**参数：**

- `nmemb`：元素个数
- `size`：单个大小

**项目用途：** 分配地址占用表并清零。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_check.c | 1 |
| hex_objdump.c | 1 |
| hex_simple.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_check.c | 46 | `int *addr_use = (int *)calloc(addr_count,sizeof(int));` |
| hex_objdump.c | 42 | `return 1;` |
| hex_simple.c | 35 | `return 1;` |

---

## exit

**原型：** `void exit(int status);`

**返回值：** 不返回，终止进程。

**参数：**

- `status`：退出码

**项目用途：** 致命错误（如内存申请失败）时立即退出。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| minirv_cpu.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| minirv_cpu.c | 9 | `exit(1);` |

---

## fclose

**原型：** `int fclose(FILE *stream);`

**返回值：** 成功0，失败EOF。

**参数：**

- `stream`：文件指针

**项目用途：** 读取/解析完成后的资源释放。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_check.c | 3 |
| hex_objdump.c | 1 |
| hex_simple.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_check.c | 30 | `fclose(f);` |
| hex_check.c | 49 | `fclose (f);` |
| hex_check.c | 90 | `fclose(f);` |
| hex_objdump.c | 102 | `if ((*line_count) % 1000 ==0){` |
| hex_simple.c | 69 | `(*inst_count)++;` |

---

## fgets

**原型：** `char *fgets(char *s, int size, FILE *stream);`

**返回值：** 成功返回s，EOF或出错返回NULL。

**参数：**

- `s`：缓冲区
- `size`：最大读取长度（含终止符）
- `stream`：输入流

**项目用途：** 读取一整行命令或文本行，便于逐行解析。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_check.c | 1 |
| hex_objdump.c | 1 |
| hex_simple.c | 1 |
| minirv_mode.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_check.c | 57 | `while (fgets(line,sizeof(line),f))` |
| hex_objdump.c | 47 | `int address_count = cpu->mem_size/4;` |
| hex_simple.c | 40 | `uint32_t instruction;` |
| minirv_mode.c | 15 | `if (fgets(command,sizeof(command),stdin)==NULL){` |

---

## fopen

**原型：** `FILE *fopen(const char *path, const char *mode);`

**返回值：** 成功返回FILE*，失败返回NULL。

**参数：**

- `path`：文件路径
- `mode`：打开模式，如"r"

**项目用途：** 在多条搜索路径中打开 hex/objdump/simple 文件以读取。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_check.c | 2 |
| hex_objdump.c | 1 |
| hex_simple.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_check.c | 26 | `FILE *f = fopen(filepath,"r");` |
| hex_check.c | 41 | `FILE *f = fopen(filepath,"r");` |
| hex_objdump.c | 45 | `void load_objdump(const char *filepath,CPU_state *cpu,int *line_count,int *inst_count,int *error_count){` |
| hex_simple.c | 38 | `void load_simple(const char *filepath, CPU_state *cpu,int *line_count,int *inst_count,int *error_count){` |

---

## fprintf

**原型：** `int fprintf(FILE *stream, const char *format, ...);`

**返回值：** 成功输出字符数；失败负值。

**参数：**

- `stream`：目标流
- `format`：格式
- `...`：可变参数

**项目用途：** （当前项目中未必出现，保留占位。）

---

## free

**原型：** `void free(void *ptr);`

**返回值：** 无返回值。

**参数：**

- `ptr`：先前分配的指针

**项目用途：** 释放模拟内存/占用表等。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_check.c | 1 |
| hex_objdump.c | 1 |
| hex_simple.c | 1 |
| minirv_cpu.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_check.c | 91 | `free(addr_use);` |
| hex_objdump.c | 103 | `printf("已处理 %d 行，加载 %d 条指令\n", *line_count, *inst_count);` |
| hex_simple.c | 70 | `}` |
| minirv_cpu.c | 21 | `free(cpu->memory);` |

---

## fseek

**原型：** `int fseek(FILE *stream, long offset, int whence);`

**返回值：** 成功0，失败非0。

**参数：**

- `stream`：文件指针
- `offset`：偏移量
- `whence`：SEEK_SET/SEEK_CUR/SEEK_END

**项目用途：** 必要时调整读指针（跳过头部/回到开头）。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_check.c | 2 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_check.c | 70 | `fseek (f,0,SEEK_SET);` |
| hex_check.c | 80 | `fseek (f,0,SEEK_SET);` |

---

## getchar

**原型：** `int getchar(void);`

**返回值：** 读到的字符（转换为int）；EOF返回EOF。

**项目用途：** 清理输入缓冲区的残留换行/杂字符。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_load.c | 5 |
| minirv_main.c | 2 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_load.c | 17 | `while ((c = getchar()) != '\n' && c != EOF);    //赋值低于比较运算法` |
| hex_load.c | 41 | `while ((c = getchar()) != '\n' && c != EOF);` |
| hex_load.c | 43 | `while ((c = getchar()) != '\n' && c != EOF);` |
| minirv_main.c | 17 | `while (getchar() !='\n');` |
| minirv_main.c | 20 | `while (getchar()!='\n');` |

---

## isspace

**原型：** `int isspace(int c);`

**返回值：** 空白为非0，否则0。

**参数：**

- `c`：字符（unsigned char 或 EOF）

**项目用途：** 裁剪/跳过空白，清洗文本。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_check.c | 2 |
| hex_simple.c | 2 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_check.c | 105 | `while(isspace((unsigned char)*start)) start++;` |
| hex_check.c | 112 | `while(isspace((unsigned char)*end)) {` |
| hex_simple.c | 7 | `if( !isxdigit((unsigned char)*p) && !isspace((unsigned char)*p) ){` |
| hex_simple.c | 19 | `*/` |

---

## isxdigit

**原型：** `int isxdigit(int c);`

**返回值：** 是十六进制字符为非0。

**参数：**

- `c`：字符（unsigned char 或 EOF）

**项目用途：** 校验简单格式行仅含十六进制字符。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_simple.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_simple.c | 7 | `if( !isxdigit((unsigned char)*p) && !isspace((unsigned char)*p) ){` |

---

## malloc

**原型：** `void *malloc(size_t size);`

**返回值：** 成功返回指针，失败NULL。

**参数：**

- `size`：字节数

**项目用途：** 为模拟内存分配缓冲区。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| minirv_cpu.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| minirv_cpu.c | 5 | `cpu->memory = (uint8_t*)malloc(cpu->mem_size); //  将内存大小转化为字节数组，并且由字节指针寻址` |

---

## memcpy

**原型：** `void *memcpy(void *dest, const void *src, size_t n);`

**返回值：** dest。

**参数：**

- `dest`：目标
- `src`：源
- `n`：字节数

**项目用途：** 将解析到的字节按小端组合成32位指令等。

---

## memmove

**原型：** `void *memmove(void *dest, const void *src, size_t n);`

**返回值：** dest。

**参数：**

- `dest`：目标
- `src`：源
- `n`：字节数

**项目用途：** 原地去除行首空白（支持重叠区域）。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_check.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_check.c | 107 | `memmove(line,start,strlen(start)+1);` |

---

## memset

**原型：** `void *memset(void *s, int c, size_t n);`

**返回值：** s。

**参数：**

- `s`：指针
- `c`：字节值
- `n`：填充长度

**项目用途：** 初始化内存与寄存器数组为0。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| minirv_cpu.c | 2 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| minirv_cpu.c | 11 | `memset(cpu->memory,0,cpu->mem_size);` |
| minirv_cpu.c | 12 | `memset(cpu->GPR,0,sizeof(cpu->GPR));` |

---

## printf

**原型：** `int printf(const char *format, ...);`

**返回值：** 成功输出的字符数；失败返回负值。

**参数：**

- `format`：格式化字符串
- `...`：可变参数，对应各占位符

**项目用途：** 命令行交互与调试打印：显示PC、指令语义、寄存器/内存状态、错误信息等。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_check.c | 12 |
| hex_execute.c | 10 |
| hex_load.c | 20 |
| hex_objdump.c | 7 |
| hex_simple.c | 7 |
| minirv_cpu.c | 4 |
| minirv_execute.c | 15 |
| minirv_gpr.c | 7 |
| minirv_main.c | 10 |
| minirv_mem.c | 6 |
| minirv_mode.c | 22 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_check.c | 13 | `printf("\n正在搜索文件: %s\n", filename);` |
| hex_check.c | 14 | `printf("搜索路径:\n");` |
| hex_check.c | 24 | `printf("  尝试: %s -> ", possible_paths[i]);` |
| hex_execute.c | 8 | `printf("输入文件名不能为空！\n");` |
| hex_execute.c | 14 | `printf("错误：无法找到hex文件 %s \n",filepath);` |
| hex_execute.c | 17 | `printf("找到文件：%s\n",filepath);` |
| hex_load.c | 4 | `printf("\n ===Hex文件加载模式 ===\n");` |
| hex_load.c | 5 | `printf("支持的文件格式：\n");` |
| hex_load.c | 6 | `printf("1. 简单格式：每行32位十六进制指令\n");` |
| hex_objdump.c | 60 | `if (objdump_check(line,&address,instructions,&count)){` |
| hex_objdump.c | 79 | `}` |
| hex_objdump.c | 89 | `}` |
| hex_simple.c | 44 | `if (address_used == NULL) return ;` |
| hex_simple.c | 48 | `while (fgets(line, sizeof(line), f)){` |
| hex_simple.c | 56 | `printf("警告: 第%d行 - 地址 0x%08x 已使用，跳过写入\n", *line_count, load_addr);` |
| minirv_cpu.c | 8 | `printf("cpu模拟内存分配失败");` |
| minirv_cpu.c | 24 | `printf("cpu执行模拟结束！内存已释放！");` |
| minirv_cpu.c | 38 | `printf("指令越界！pc=0x%08x,inst=0x%08x\n",cpu->pc,cpu_inst);` |
| minirv_execute.c | 4 | `printf("当前pc=0x%08x, Inst=0x%08x\n",cpu->pc,mem_read(cpu,cpu->pc));` |
| minirv_execute.c | 16 | `printf("未知执行指令！funct3=0x%02x",inst.funct3);` |
| minirv_execute.c | 30 | `printf("未知执行指令！funct3=0x%02x",inst.funct3);` |
| minirv_gpr.c | 13 | `printf("输入读取寄存器无效！pc=0x%08x, reg=x%d\n",cpu->pc,reg);` |
| minirv_gpr.c | 23 | `printf("输入读取寄存器无效！pc=0x%08x, reg=x%d\n",cpu->pc,reg);` |
| minirv_gpr.c | 27 | `printf("寄存器x0不能写入！\n");` |
| minirv_main.c | 3 | `printf("=== MiniRV RISC-V 模拟器启动 ===\n");` |
| minirv_main.c | 8 | `printf("\n=== 主菜单 ===\n");` |
| minirv_main.c | 9 | `printf("1. 指令功能测试模式\n");` |
| minirv_mem.c | 7 | `printf("字输入读取地址对齐失败！ pc=0x%08x\n",addr);` |
| minirv_mem.c | 10 | `printf("字内存读取地址越界！pc=0x%08x\n",addr);` |
| minirv_mem.c | 20 | `printf("字输入读写地址对齐失败！ pc=0x%08x\n",addr);` |
| minirv_mode.c | 6 | `printf("\n=== 进入单步调试模式 ===\n");` |
| minirv_mode.c | 7 | `printf("命令: s(单步) r(运行) d(寄存器) m(内存) q(退出)\n");` |
| minirv_mode.c | 13 | `printf("\n[Inst=%d,pc=0x%08x] > : ",step_count,cpu->pc);` |

---

## putchar

**原型：** `int putchar(int c);`

**返回值：** 写入字符或EOF。

**参数：**

- `c`：字符

**项目用途：** （当前项目中未必出现，保留占位。）

---

## puts

**原型：** `int puts(const char *s);`

**返回值：** 成功非负，失败EOF。

**参数：**

- `s`：字符串

**项目用途：** （当前项目中未必出现，保留占位。）

---

## realloc

**原型：** `void *realloc(void *ptr, size_t size);`

**返回值：** 新指针或NULL。

**参数：**

- `ptr`：原指针
- `size`：新大小

**项目用途：** （当前项目中未必出现，留作扩展。）

---

## scanf

**原型：** `int scanf(const char *format, ...);`

**返回值：** 成功匹配并赋值的项数。

**参数：**

- `format`：格式化字符串
- `...`：指向目标变量的指针

**项目用途：** 主菜单/简单问答输入，和printf组成简易TUI。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_load.c | 3 |
| minirv_main.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_load.c | 11 | `if (scanf("%s",filename) !=1)` |
| hex_load.c | 38 | `if (scanf("%d",&execute_mode) !=1)` |
| hex_load.c | 67 | `if (scanf("%s",retry) == 1 && strcmp(retry,"yes") == 0)` |
| minirv_main.c | 14 | `if (scanf("%d",&mode) !=1)` |

---

## sscanf

**原型：** `int sscanf(const char *str, const char *format, ...);`

**返回值：** 成功匹配并赋值的项数。

**参数：**

- `str`：源字符串
- `format`：格式
- `...`：输出变量指针

**项目用途：** 从命令文本解析十六进制地址、数字参数。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| minirv_mode.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| minirv_mode.c | 42 | `if (sscanf(command+2,"%x",&addr)==1)      // sscanf(addr*,"mode",*addr)第一个是读取地址，第二个写入地址` |

---

## strchr

**原型：** `char *strchr(const char *s, int c);`

**返回值：** 首个匹配位置或NULL。

**参数：**

- `s`：字符串
- `c`：字符

**项目用途：** objdump 行中定位冒号分隔的地址字段。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_check.c | 2 |
| hex_objdump.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_check.c | 17 | `if (strchr(possible_paths[i],'%') != NULL)` |
| hex_check.c | 99 | `char *comment = strchr(line,'#');` |
| hex_objdump.c | 5 | `char *colon = strchr(line,':');` |

---

## strcmp

**原型：** `int strcmp(const char *s1, const char *s2);`

**返回值：** 相等0；小于/大于返回负/正。

**参数：**

- `s1`：字符串1
- `s2`：字符串2

**项目用途：** 命令字精确匹配或简单回答判断。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_load.c | 3 |
| minirv_mode.c | 4 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_load.c | 19 | `if (strcmp(filename,"q") ==0 \|\| strcmp(filename,"Q") == 0)` |
| hex_load.c | 67 | `if (scanf("%s",retry) == 1 && strcmp(retry,"yes") == 0)` |
| minirv_mode.c | 20 | `if (strcmp(command, "s") == 0){` |
| minirv_mode.c | 33 | `} else if (strcmp(command,"r")==0){` |
| minirv_mode.c | 36 | `} else if (strcmp(command,"d")==0)` |

---

## strcpy

**原型：** `char *strcpy(char *dest, const char *src);`

**返回值：** dest。

**参数：**

- `dest`：目标
- `src`：源

**项目用途：** （当前项目中未必出现，保留占位。）

---

## strlen

**原型：** `size_t strlen(const char *s);`

**返回值：** 字符串长度（不含\0）。

**参数：**

- `s`：C字符串

**项目用途：** 校验空行/短行，配合文本清洗与解析。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_check.c | 1 |
| hex_execute.c | 1 |
| minirv_mode.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_check.c | 107 | `memmove(line,start,strlen(start)+1);` |
| hex_execute.c | 6 | `if (filename == NULL \|\| strlen(filename) == 0)` |
| minirv_mode.c | 58 | `}else if (strlen(command)>0){` |

---

## strncmp

**原型：** `int strncmp(const char *s1, const char *s2, size_t n);`

**返回值：** 比较前n个字符的结果。

**参数：**

- `s1`：字符串1
- `s2`：字符串2
- `n`：比较长度

**项目用途：** 命令前缀判断（如"m "开头）。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| minirv_mode.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| minirv_mode.c | 39 | `} else if (strncmp(command,"m ",2)==0)        // 从数组其实地址开始读取num个前缀字符` |

---

## strncpy

**原型：** `char *strncpy(char *dest, const char *src, size_t n);`

**返回值：** dest。

**参数：**

- `dest`：目标缓冲
- `src`：源
- `n`：最大拷贝数

**项目用途：** 安全抽取地址子串以便转换。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_check.c | 1 |
| hex_objdump.c | 2 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_check.c | 21 | `strncpy(filepath,possible_paths[i],path_size);` |
| hex_objdump.c | 9 | `strncpy(line_addr,line,colon-line);` |
| hex_objdump.c | 19 | `strncpy(data_copy,data_start,sizeof(data_copy)-1);` |

---

## strtol

**原型：** `long strtol(const char *nptr, char **endptr, int base);`

**返回值：** 转换后的长整型；失败返回0。

**参数：**

- `nptr`：数字串
- `endptr`：停止位置输出
- `base`：进制

**项目用途：** （当前项目中未必出现，留作扩展。）

---

## strtoul

**原型：** `unsigned long strtoul(const char *nptr, char **endptr, int base);`

**返回值：** 转换后的无符号长整型；失败返回0。

**参数：**

- `nptr`：数字串
- `endptr`：停止位置输出
- `base`：进制

**项目用途：** 将地址/字节token从16进制文本转成数值。

**各文件使用次数：**

| 文件 | 次数 |
|:------|------:|
| hex_objdump.c | 2 |
| hex_simple.c | 1 |

**示例出现位置（最多3条/文件）：**

| 文件 | 行号 | 代码片段 |
|:------|:----:|:----------|
| hex_objdump.c | 13 | `uint32_t word_addr = (uint32_t)strtoul(line_addr,&endptr,16);` |
| hex_objdump.c | 27 | `*/` |
| hex_simple.c | 13 | `while(*p){` |

---
