1、静态数据返回类型：只属于当前.c文件下可用，无法被其他.c文件访问。
2、文件打开和关闭函数：
    fopen,打开文件；fclose，关闭文件
    模式参数：
        r:
        w:
        a:
        rb:
        wb:
        r+:
    正常读完文件之后，必须关闭文件，防止系统资源泄露
3、使用man查看步骤
    1、man的分区组织
        1：用户命令
        2：系统调用（内核接口，如 open(2)）
        3：C 库函数（如 strchr(3)、printf(3)
        7：杂项/概念（协议、标准、宏观说明）
        绝大多数 C 函数用 3 区；内核调用用 2 区
    2、先查分区/名字是否存在
        whatis <名字>   / man -f <名字>
    3、根据分区打开对应手册页
        man <分区> <名字>
    4、在man页面内阅读与导航
        space,下翻，b上翻
        SYNOPSIS：需要的头文件（通常是 #include <string.h>），函数原型。
        DESCRIPTION：行为语义、参数含义、边界条件（比如未找到返回 NULL）。
        RETURN VALUE：成功/失败返回值。
        ERRORS：可能的 errno（一些库函数不设 errno）。
        ATTRIBUTES：线程安全性（MT-Safe 等）。
        CONFORMING TO：是否符合 C/POSIX 标准以及差异。
        NOTES / BUGS：实现细节，特性宏（如 glibc 的 feature test macros）。
        EXAMPLES：示例代码。
        SEE ALSO：相近函数（如 strrchr(3)、memchr(3)）。


4、加载程序的总体设计：
    [外层交互壳]
    └─ hex_load(cpu)
        ├─ 读取文件名、清输入缓冲
        ├─ find_hex_file()    // 在若干目录中定位真实路径
        ├─ load_hex_file()    // 真正把文本解析为指令并写入内存
        └─ 选择调试/运行：debug_mode() / run_mode() / 返回

    [内层加载引擎]
    └─ load_hex_file(cpu, path)
        ├─ FILE* 打开文件
        ├─ address_used[] 标记数组(检测覆盖)
        ├─ 逐行循环：
        │   ├─ clean_line()                 // 去换行/注释/空白
        │   ├─ parse_objdump_line()  或     // “字地址 + 字节流”
        │   │   parse_simple_hex_line()     // “一行 = 一个32位指令”
        │   ├─ 地址变换/合法性检查(×4、对齐、越界、覆盖)
        │   └─ mem_write(cpu, addr, inst)   // 写入模拟内存
        ├─ 关闭文件, 释放 address_used
        └─ 打印统计(格式类型/总指令数/告警)

5、控制流：从输入文件名到进行执行
    用户输入文件名 ──> hex_load()
       ├─ find_hex_file()  // 尝试 "./", "load/", "../load/", "test/", ...
       ├─ load_hex_file()  // 成功则：
       │   ├─ fopen() 打开文本
       │   ├─ for (line in file):
       │   │   ├─ clean_line(line)
       │   │   ├─ if parse_objdump_line(line):   // 多指令/行
       │   │   │     addr = word_addr * 4
       │   │   │     for each inst: 检查→mem_write()
       │   │   └─ else if parse_simple_hex_line(line): // 一指令/行
       │   │         addr = 当前写指针(或行内显式地址)
       │   │         检查→mem_write(); 写指针+=4
       │   └─ fclose() + 统计/告警输出
       └─ 用户选择：debug_mode() 或 run_mode() 或返回

6、执行逻辑
    hex_load()
      ├─ find_hex_file(filename → found_path)
      └─ load_hex_file(cpu, found_path)
            ├─ fopen()
            ├─ while (fgets line):
            │    clean_line(line)
            │    if parse_objdump_line(line → addr, insts[], count):
            │        addr *= 4; for i in [0..count]: check+write(addr+4*i, insts[i])
            │    else if parse_simple_hex_line(line → inst):
            │        check+write(curr_addr, inst); curr_addr += 4
            │    else: warn("无法识别的行")
            └─ fclose() + stats

7 文件搜索
    1 输入相对路径(文件名)与预设路径进行拼接，然后遍历路径检查文件是否存在。
    2 利用指针数组预设路径，利用占位符：%s 与snprintf拼接完整路径。
    3 利用file_exists(test_paths)函数遍历拼接路径是否存在文件，用于验证搜索文件路径的正确性路径。
    4 文件指针 FILE *f:
        1 属于文件控制块，操作系统内部管理的数据结构
        2 相当于一种结构体，包含文件很多内容：文件路径，文件状态，文件属性等等
    5 fopen  :  FILE * fopen ( const char * filename, const char * mode );
        文件控制函数，会返回"文件指针"用于表示文件的结构状态
        根据返回值是否为NULL判断文件存在状态
    6 fclose ：int fclose ( FILE * stream );
        文件关闭函数，唯一参数，文件指针，返回int 值标签。
8 字符串拼接
    

9 fgets         char *fgets(char s[restrict .size], int size, FILE *restrict stream);
    fgets 会执行以下操作：
    从文件指针 f 当前所在的位置开始读取数据。
    读取字符并将其存入 line 数组中，直到遇到以下之一：
    换行符 (\n)：表示这一行的结束。
    文件结束符 (EOF)：表示文件已经结束。
    读取的字符数达到指定的大小：例如，如果 line 数组的大小为 512 字符，fgets 会读取最多 511 个字符（最后一个位置会留给 \0，确保字符串正确结束）。
    fgets 会将文件中的每行（包括换行符 \n）存储到 line 数组中

10 strchr       char *strchr(const char *s, int c);
    功能：strchr 用于在字符串中查找 第一次出现的指定字符。它从字符串 s 的开头开始搜索，直到找到字符 c。
    参数：
        s：目标字符串。
        c：需要查找的字符（以 int 传入，但会被转换为字符）。
        返回值：返回指向找到的字符的指针。如果没有找到字符，返回 NULL。

11 strspn       size_t strspn(const char *s, const char *accept); 
    功能：strspn 用于计算字符串 s 中从开头开始，连续包含在 accept 字符串中的字符的长度。
    参数：
        s：目标字符串。
        accept：一个包含可接受字符的字符串。
        返回值：返回 s 开头部分与 accept 中字符匹配的最大长度。

12 strcpy       char *strcpy(char *restrict dst, const char *restrict src); 
    功能：strcpy 用于将源字符串 src 复制到目标字符串 dst 中。
    参数：
        dst：目标字符串（必须有足够的空间来存储源字符串）。
        src：源字符串。
        返回值：返回目标字符串 dst 的指针。  
        缺陷：不自动添加末尾'\0'

13 strncpy      char *strncpy(char dst[restrict .dsize], const char *restrict src,size_t dsize);    
    功能：strncpy 用于将源字符串 src 中的前 dsize 个字符复制到目标字符串 dst 中。它是 strcpy 的更安全版本，因为它允许限制复制的字符数。
    参数：
        dst：目标字符串。
        src：源字符串。
        dsize：要复制的最大字符数。
        返回值：返回目标字符串 dst 的指针。
        缺陷：不自动添加末尾'\0'

14 strcspn      size_t strcspn(const char *s, const char *reject);
    功能：strcspn 用于计算字符串 s 中从开头开始，直到遇到 reject 字符串中的任何一个字符为止的部分的长度。
    参数：
        s：目标字符串。
        reject：包含拒绝字符的字符串。
        返回值：返回 s 中没有被拒绝字符的部分的长度。

15 strcmp       int strcmp(const char *s1, const char *s2); 
   strncpy      int strncmp(const char *str1, const char *str2, size_t n);  增加了比较的数量
    功能：strcmp 逐个比较字符串的字符，从第一个开始
    参数：
        s1 s2 两个字符串指针，指向一个存放字符串的地址
        返回值 < 0：在第一个不匹配的字符处，str1 中的字符的ASCII值小于 str2 中对应字符的ASCII值。或者，str1 是 str2 的一个真前缀（例如："abc" 和 "abcd"）。
        返回值 == 0：两个字符串的内容完全相同，包括长度和每个字符。
        返回值 > 0：在第一个不匹配的字符处，str1 中的字符的ASCII值大于 str2 中对应字符的ASCII值。或者，str2 是 str1 的一个真前缀（例如："abcd" 和 "abc"）。
16 snprintf     int snprintf(char str[restrict .size], size_t size,const char *restrict format, ...);
    功能：snprintf 用于将格式化的输出写入字符串中。它的功能类似于 sprintf，但是它增加了一个安全性检查，通过 size 限制了写入的字符数，防止缓冲区溢出。
    参数：
        str：目标字符数组，用来存储格式化后的结果。
        size：目标数组 str 的大小，snprintf 会确保写入的字符数不超过这个大小。
        format：格式化字符串，类似于 printf 的格式控制字符串。

    ...：要格式化的值。

    返回值：返回写入的字符数（不包括结尾的 \0）。如果字符数超过 size，则返回写入的字符数，但实际写入的字符数不会超过 size - 1。
总结：
    strchr：查找字符串中第一次出现的指定字符。
    strspn：计算字符串中从开头开始，连续包含在另一个字符串中的字符的长度。
    strcpy：复制一个字符串到另一个字符串，直到遇到 \0。
    strncpy：类似 strcpy，但允许你指定最多复制多少字符，防止溢出。
    strcspn：计算字符串中从开头开始，到遇到某个字符集中的任意字符为止的长度。
    snprintf：格式化输出到一个字符串中，并限制输出的字符数，防止缓冲区溢出。

17 isspace      int isspace(int c);
    空白符：
        空格 (' '),
        制表符 ('\t'),
        换行符 ('\n'),
        回车符 ('\r'),
        垂直制表符 ('\v'),
        换页符 ('\f').
    参数：c 要检查的字符，它会被转换为 unsigned char 类型，或者是 EOF。
    返回值：
        如果字符是空白字符，返回非零值（通常为 1）。
        如果字符不是空白字符，返回 0。


18 memmove      void *memmove(void dest[.n], const void src[.n], size_t n);
    功能： 用于从源内存区域 src 复制 n 个字节到目标内存区域 dest。与 memcpy 不同，memmove 会处理内存重叠的情况，确保即使源区和目标区重叠也能正确复制。
    参数：
        dest：目标内存区域的指针。
        src：源内存区域的指针。
        n：要复制的字节数。
        返回值： 返回目标内存区域 dest 的指针。
    特点： 能够安全地处理内存重叠的复制操作，比 memcpy 更安全但可能稍慢。

19 fseek        int fseek ( FILE * stream, long int offset, int origin );
    功能：fseek 用于重新定位文件流 stream 的文件位置指示器。可以设置文件读/写位置到指定偏移量处。
    参数：
        stream：文件指针。
        offset：相对位移（可正可负）。
        origin：基准点：SEEK_SET(文件头) / SEEK_CUR(当前位置) / SEEK_END(文件尾)。
        返回值：成功返回 0；失败返回非零（如超出范围、流不支持定位）。

20 strtoul      unsigned long strtoul(const char *restrict nptr,char **restrict endptr, int base);
    功能：strtoul 将字符串按指定数制格式解析
    参数：
        nptr：待解析的字符串（可含前导空白、可选 +/- 号）。
        endptr：若非 NULL，返回第一个未被解析的字符指针（用于检测解析到哪儿）。
        base：进制。0=自动（0x/0X→16 进制，前导 0→8 进制，否则 10 进制）；或显式 2..36。
        返回值：解析得到的 unsigned long 
    实例：
        strtoul(addr_str, &endptr, 16)
        从 addr_str 开始读取字符串内容，
        按 16 进制 规则解析出一个无符号整数，
        并在解析停止的地方记录下那个字符的地址到 endptr 中。
    注意/缺陷：
        溢出：返回 ULONG_MAX 并将 errno 置为 ERANGE。
        endptr == nptr 表示一个数字都没读到（常用于判错）。
        允许前导空白与 +/-；若有 -，结果按无符号规则取模，通常不应给无符号解析负数
21 strtok       char *strtok(char *restrict str, const char *restrict delim);
    功能：字符串 str 分割成一系列标记（token）。首次调用时传入要分割的字符串，后续调用传入 NULL 继续分割同一字符串。
    参数：
        str：要分割的字符串（第一次调用时指定，后续调用为 NULL）。
        delim：包含所有分隔符的字符串。
        返回值： 返回指向下一个标记的指针，如果没有更多标记则返回 NULL。
    缺陷：
        会修改原始字符串，用 '\0' 替换分隔符
        不是线程安全的
        内部使用静态变量保存状态

22 calloc       void *calloc(size_t nmemb, size_t size);
    功能：calloc 用于动态分配内存，并自动初始化为零。它分配一个足够大的内存块来容纳 nmemb 个元素，每个元素的大小为 size 字节。
    参数：
        nmemb：要分配的元素数量。
        size：每个元素的大小（字节）。
        返回值： 成功时返回指向分配内存的指针，失败返回 NULL。
        特点： 分配的内存会自动初始化为全零，提供干净的内存空间。

**********x86_64 安装riscv64编译器***********
1 包管理器安装：
    sudo apt update
    sudo apt install gcc-rsicv64-linux-gnu
    验证：riscv64-linux-gnu-gcc --version
2 源码编译依赖：
        # 安装依赖
    sudo apt install build-essential git autoconf automake autotools-dev curl python3 libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev
        # 克隆工具链源码
    git clone --recursive https://github.com/riscv-collab/riscv-gnu-toolchain
    cd riscv-gnu-toolchain   
        # 配置并编译（这会花费较长时间）
    ./configure --prefix=/opt/riscv --enable-multilib
    make linux -j$(nproc)
        # 添加到PATH
    echo 'export PATH=/opt/riscv/bin:$PATH' >> ~/.bashrc
    source ~/.bashrc
3   编译
    未指定动态静态编译，默认动态编译，在qemu模拟器中，两者执行方式不一样
    1 静态编译：
        riscv64-linux-gnu-gcc -static -g <.c原文件> -o<可执行文件名>
    2 使用动态链接库：
        安装动态链接库：sudo apt install libc6-riscv64-cross
        正常编译不加static：riscv64-linux-gnu-gcc -g <.c原文件> -o<可执行文件名>
    3 设置编译别名：
        echo "alias rv64_gcc='riscv64-unknown-linux-gnu-gcc'" >> ~/.bashrc
        source ~/.bashrc
    4 别名编译：rv64_gcc -g <.c原文件> -o<可执行文件名>

4   执行
    1 执行可执行文件需要指向动态链接库：qemu-riscv64 -L /usr/riscv64-linux-gnu ./<可执行文件名>
    2 为动态库设置环境路径
        echo 'export QEMU_LD_PREFIX=/usr/riscv64-linux-gnu' >> ~/.bashrc
        source ~/.bashrc
    3 设置执行别名：
        echo "alias rv64_qemu='qemu-riscv64 -L /usr/riscv64-linux-gnu'" >> ~/.bashrc
        source ~/.bashrc
    4 别名执行：rv64_qemu ./<可执行文件名>


*************** qemu 模拟器安装 *************
1 包管理器安装：
    sudo apt update
    # 安装 QEMU 用户模式和系统模式
    sudo apt install qemu-user qemu-user-static qemu-system-misc
    # 安装其他有用的工具
    sudo apt install qemu-utils
2 源码安装：
    # 安装编译依赖
    sudo apt install build-essential git libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev ninja-build
    cd ~
    git clone https://git.qemu.org/git/qemu.git
    cd qemu
    git checkout v8.0.0  # 使用稳定版本
    # 配置和编译
    ./configure --target-list=riscv64-softmmu,riscv64-linux-user --enable-static
    make -j$(nproc)
    # 安装
    sudo make install

*************** qemu-gdb-调试步骤 *************
1 终端1 启动qemu远程服务器并等待gdb连接
    rv64_qemu -g 1234 ./<可执行文件>
2 终端2 启动gdb并连接
    rv64_gdb ./<可执行文件>                       # rv64_gdbd 别名设置
    (gdb) target remote localhost:1234          # 在 GDB 中连接远程目标