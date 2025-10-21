#include "../head/head.h"
// 检查文件是否存在   // 成功返回1 失败0 
// 静态数据返回类型，属于当前.c文件专用。其他点c文件无法访问
int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

// 尝试多种可能的文件路径    返回1说明，查找成功，返回0查找文件失败
int find_hex_file(const char *filename, char *found_path, size_t path_size) {
    const char *possible_paths[] = {
        filename,                    // 直接路径
        "load/%s",                   // load子目录
        "../load/%s",               // 上级load目录
        "hex/%s",                   // hex子目录
        "../hex/%s",                // 上级hex目录
        "test/%s",                  // test子目录
        "../test/%s",               // 上级test目录
        NULL
    };
    
    for (int i = 0; possible_paths[i] != NULL; i++) {
        char test_path[256];   
        // 判断预设路径是否存在占位符，用于后续拼接
        if (strchr(possible_paths[i], '%') != NULL) {
        // 输入文件名与预设路径进行拼接形成完整文件搜索路径
        // 将possible_paths[i]中的 “%s” 替换成filename 写入到test_path，sizeof(test_path)防止越界超过test_path数组大小
        // snprintf 通过读取路径模板（带有 %s）和提供的文件名（filename）生成一个新的路径,写入到一个缓冲区，test_Path
            snprintf(test_path, sizeof(test_path), possible_paths[i], filename);
        } else {

            strncpy(test_path, possible_paths[i], sizeof(test_path));
        }
        // 遍历拼接路径去尝试打开文件，检查是否真实存在，能打开便说明真实存在。说明找到加载文件了。
        if (file_exists(test_path)) {
            strncpy(found_path, test_path, path_size);
            return 1;
        }
    }
    
    return 0;
}

/*
指定路径查询输入文件是否存在
1、利用指针数组建立预设查询路径：const char *possible_paths[]={……};

!!!!!!!:    %s 是C程序格式占位符，它的作用是动态替换。通过 snprintf，我们能够将 filename 插入到路径中，生成一个完整的文件路径。

2、函数：strchr:    char *strchr(const char *s, int c);
    功能：找字符串s中寻找第一次出现的字符c,并返回该字符的出现的指针(返回其地址)，若没有找到返回NULL
3、函数：snprintf:  int snprintf（char * str，size_t n，const char * format，...）；
    功能：格式字符串与变量结合，生成一个新的字符串，并将结果写入指定的缓冲区
    str：目标缓冲区，将拼接的结果写入这里。
    size：目标缓冲区的大小，防止超出边界。
    format：格式字符串，可以包含普通文本和格式说明符（比如 %s, %d 等）。
    ...：传入的变量，按格式说明符插入到目标字符串中。
4、函数：strcnpy:   char *strncpy(char *dest, const char *src, size_t n);

*/  