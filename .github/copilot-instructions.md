# Minirv_emu AI 编码指南

## 1. 项目背景与架构

**目标**: 使用 C 语言实现的极简 RISC-V (RV32I) 指令集模拟器。

### 核心组件
- **状态管理**: `CPU_state` (定义于 `head/head.h`) 是传递给几乎所有函数的中心上下文对象。它包含通用寄存器 (`GPR`)、内存指针、`pc` 和运行标志。
- **执行循环**: 在 `src/minirv_cpu.c` 的 `cpu_core` 函数中实现。遵循标准的 取指 (Fetch) -> 译码 (Decode) -> 执行 (Execute) 周期。
- **内存**: 模拟为单一的扁平字节数组 (`cpu->memory`)，在 `cpu_init` 中分配。访问通过 `mem_read`/`mem_write` (32位) 和 `byte_read`/`byte_write` 进行中介。
- **译码**: `src/minirv_decode.c` 将原始机器码转换为包含解析字段（操作码, rd, rs1, funct3 等）的 `DecodeInst` 结构体。
- **指令实现**: 单个指令（如 `lw`, `add`）的具体逻辑在 `src/minirv_execute.c` 中实现，并通过 switch 语句进行分发。

### 逻辑架构

#### 1. 数据结构 (Data Structures)
- **CPU 全局状态 (`CPU_state`)**: 系统的“上帝对象”，保存运行时上下文（GPR, Memory, PC）。
- **译码结果包 (`DecodeInst`)**: 拆解后的指令中间件，包含 Opcode, rd, rs1, funct3 及符号扩展后的 Immediate。

#### 2. 系统分层 (System Layering)
- **交互层**: `minirv_main.c` (CLI 菜单, 模式选择)。
- **核心层**: `minirv_cpu.c` (Fetch-Decode-Execute 主循环, 内存管理)。
- **逻辑层**: `minirv_decode.c`, `minirv_execute.c` (无状态计算)。
- **存储层**: `minirv_mem.c`, `minirv_gpr.c` (底层读写封装)。

#### 3. 执行功能逻辑 (Execution Logic)
位于 `cpu_core` 的心跳循环：
1.  **取指 (Fetch)**: `mem_read(pc)` 获取指令，检查边界。
2.  **译码 (Decode)**: `decode_inst` 提取字段并处理立即数符号扩展。
3.  **执行 (Execute)**: `inst_execute` 经由 Opcode -> Funct3 二级分发调用具体指令函数 (如 `lw`)，完成计算并写回结果。

### 目录结构
- `src/`: 核心逻辑 (cpu, decode, execute, memory, main)。
- `head/`: 头文件定义 (`head.h` 是主要接口)。
- `load/`: 用于解析及加载外部 `.hex` 程序文件的逻辑。
- `test/`: 内置指令验证测试。

## 2. 开发工作流

### 构建与运行
- **构建系统**: 普通 GNU `makefile`。
- **命令**: 运行 `make` 进行构建。输出文件为 `bin/Minirv_emu`。
- **清理**: `make clean` 删除目标文件和二进制文件。
- **运行**: `make run` 或直接运行 `./bin/Minirv_emu`。

### 交互模式
`main` 函数 (`src/minirv_main.c`) 提供了一个 CLI 菜单：
1. **测试模式 (Test Mode)**: 运行内部验证 (`inst_test.c`)。
2. **Hex 模式 (Hex Mode)**: 加载外部机器码（基于文本的 hex 格式）到内存并执行。

## 3. 编码规范

- **语言标准**: C99 (`-std=c99`)。
- **数据类型**: 严格使用 `<stdint.h>` 类型 (`uint32_t`, `int32_t`, `uint8_t`) 用于所有架构元素（寄存器、地址、操作码），以确保位宽正确。
- **头文件**:
  - 在源文件中始终包含 `../head/head.h`。
  - `head.h` 是此项目的统括性头文件；除非必要，避免随意创建新头文件。
- **常量**: 在 `head/head.h` 中使用 `#define` 定义架构常量（Opcodes, Funct3/7）。
- **错误处理**:
  - 使用 `printf` 将错误打印到 `stdout`。
  - 通过设置 `cpu->cpu_running = 0` 或返回非零状态码来发出实现错误信号。

## 4. 具体实现细节

- **内存访问**: 使用 `mem_read(cpu, addr)` 和 `mem_write(cpu, addr, val)` 进行对齐的字访问。不要在 `minirv_mem.c` 之外直接解引用 `cpu->memory`。
- **符号扩展**: 实现立即数指令（如 `addi`）时，如果立即数存储在较小的类型中，请确保显式处理符号扩展，尽管 `DecodeInst` 对 `imm` 使用了 `int32_t`。
- **字节序 (Endianness)**: 模拟器在宿主架构（通常是小端序）上运行，模拟 RISC-V（小端序）。直接字节转换通常是安全的，但 `minirv_mem.c` 处理具体的字节组装。

### 示例：添加新指令
1. 在 `head/head.h` 中定义 `OPCODE_*` 和 `FUNCT3_*`。
2. 更新 `src/minirv_decode.c` 中的 `decode_inst` 以解析该格式。
3. 在 `src/minirv_execute.c` 中实现逻辑函数（例如 `void sub(...)`）。
4. 在 `inst_execute` 分发函数中注册该 case。

## 5. 工作区使用到的标准库函数（按头文件整理）

### `<stdio.h>`
- `printf`: 原理为格式化输出到标准输出流；参数为格式字符串与可变参数；返回写入字符数，失败返回负值。
- `scanf`: 原理为从标准输入解析格式化数据；参数为格式字符串与变量地址；返回成功匹配/赋值的项数。
- `sscanf`: 原理为从字符串解析格式化数据；参数为源字符串、格式字符串与变量地址；返回成功匹配/赋值的项数。
- `snprintf`: 原理为将格式化输出写入缓冲区并限制长度；参数为缓冲区、最大长度、格式字符串与可变参数；返回“本应写入”的字符数（不含结尾 `\0`）。
- `getchar`: 原理为从标准输入读取单个字符；无参数；返回读取到的字符（以 `int` 表示）或 `EOF`。
- `fgets`: 原理为从流读取一行或指定长度文本；参数为缓冲区、长度、文件流；返回缓冲区指针或 `NULL` 表示失败/EOF。
- `fopen`: 原理为打开文件流；参数为路径与模式字符串；返回 `FILE*` 或 `NULL`。
- `fclose`: 原理为关闭文件流并刷新缓冲区；参数为 `FILE*`；返回 `0` 成功，`EOF` 失败。

### `<stdlib.h>`
- `malloc`: 原理为从堆分配指定字节的连续内存；参数为字节数；返回内存指针或 `NULL`。
- `free`: 原理为释放堆内存；参数为 `malloc`/`calloc`/`realloc` 返回的指针；无返回值。
- `exit`: 原理为终止进程并向宿主返回状态码；参数为退出码；无返回值（不返回）。

### `<string.h>`
- `memset`: 原理为按字节设置内存区域；参数为目标指针、填充值、字节数；返回目标指针。
- `memcpy`: 原理为按字节复制内存（不支持重叠）；参数为目标指针、源指针、字节数；返回目标指针。
- `memmove`: 原理为按字节复制内存（支持重叠）；参数为目标指针、源指针、字节数；返回目标指针。
- `strlen`: 原理为遍历到 `\0` 统计长度；参数为 C 字符串；返回字符串长度（不含 `\0`）。
- `strcmp`: 原理为逐字节比较字符串；参数为两个 C 字符串；返回小于/等于/大于 0 表示字典序关系。
- `strncmp`: 原理为最多比较前 `n` 个字符；参数为两个字符串与比较长度；返回值同 `strcmp`。
- `strncpy`: 原理为最多复制前 `n` 字节到目标缓冲区；参数为目标指针、源字符串、最大长度；返回目标指针（可能不含结尾 `\0`）。
- `strchr`: 原理为查找字符串中首次出现的字符；参数为字符串与目标字符；返回指向该字符的指针或 `NULL`。
- `strtok`: 原理为按分隔符切分字符串并维护内部状态；参数为待分割字符串/`NULL`、分隔符字符串；返回下一个 token 指针或 `NULL`。

### `<ctype.h>`
- `isxdigit`: 原理为判断字符是否为十六进制数字；参数为 `unsigned char` 或 `EOF`；返回非 0 表示真。
- `isspace`: 原理为判断字符是否为空白符；参数为 `unsigned char` 或 `EOF`；返回非 0 表示真。
