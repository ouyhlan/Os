#include "libc.h"

/* Compare two string 
** 0 for same, -1 for not the same  
*/
int strncmp(char *str1, char *str2, unsigned int n) {
    for (int i = 0; i < n; ++i) {
        if (str1[i] != str2[i])
            return -1;
    }
    return 0;
}

int strlen(char *str) {

    int count = 0;
    while (str[count++] != '\0') {}
    return count - 1;
}

char getchar() {
    char res;
    asm("int $0x80\n\t":"=a"(res):"a"(2):);
    return res;
}

void gets(char *buf) {
    asm("int $0x80\n\t"::"a"(3), "D"(buf):);
}

/* 暂时不提供异常处理 */
void scanf(char *str, ...) {
    va_list ap;
    int str_len = strlen(str), arg_len = 0;

    /* 计算输入的参数 */
    for (int i = 0; i < str_len; ++i) {
        if (str[i] == '%') {
            arg_len++;
        }
    }

    va_start(ap, arg_len);
    for (int i = 0; i < str_len; ++i) {
        if (str[i] == '%') {
            char ch;
            while ((ch = getchar()) == ' ' || ch == '\n') {  /* 忽略开头的空格 */
                /* do nothing */;
            }

            /* 处理数字输出 */
            if (str[i + 1] == 'd') {
                /* 处理输入 */
                int dig_pos = 0, num_res = 0;
                char digit_buf[9] = {0};   /* 一般int只有9位 */
                for (ch; (ch >= '0' && ch <= '9'); ch = getchar()) {
                    digit_buf[dig_pos++] = ch;
                }

                /* str2int */
                for (int j = 0; j < dig_pos; ++j) {
                    num_res = num_res * 10 + (digit_buf[j] - '0');
                }

                /* 赋值语句 */
                int *scanf_num = va_arg(ap, int *);
                *scanf_num = num_res;
            }
            else if (str[i + 1] == 's') {
                char *scanf_str = va_arg(ap, char*);
                int str_pos = 0;
                for (ch; ch != ' ' && ch != '\n'; ch = getchar()) {
                    scanf_str[str_pos++] = ch;
                }
                scanf_str[str_pos] = '\0';
            }
            ++i;    /* 跳过%后面的字符 */
        }
    }
}

/* 显示一个字符 */
void putc(char c) {
    asm("int $0x80\n\t"::"a"(0), "D"(c):);
}

/* 显示一段字符串 */
void puts(char *str) {
    asm("int $0x80\n\t"::"a"(1), "D"(str):);
    putc('\n');
}

/* 仅支持非负整数、字符串输出
** 暂时不支持负数、浮点数输出
** 还没有错误处理！！！
*/
void printf(char *str, ...) {
    va_list ap;
    int str_len = strlen(str), arg_len = 0;

    for (int i = 0; i < str_len; ++i) {
        if (str[i] == '%') {
            arg_len++;
        }
    }

    va_start(ap, arg_len);

    for (int i = 0; i < str_len; ++i) {
        if (str[i] == '%') {
            /* 处理数字输出 */
            if (str[i + 1] == 'd') {
                int digit_temp = va_arg(ap, int); /* 读取参数 */
                /* int2str */
                int dig_pos = 0;
                char digit_buf[9] = {0};   /* 一般int只有9位 */
                while (digit_temp > 0) {
                    digit_buf[dig_pos++] = digit_temp % 10 + '0';
                    digit_temp /= 10;
                }
                for (int j = dig_pos - 1; j >= 0; --j) {
                    putc(digit_buf[j]);
                }
            }
            else if (str[i + 1] == 's') {
                char *print_str = va_arg(ap, char*);
                int index = 0;
                while (print_str[index] != '\0') {
                    putc(print_str[index++]);
                }
            }
            ++i;    /* 跳过%后面的字符 */
        }
        else {
            putc(str[i]);
        }
    }
    va_end(ap);
}

void exec(unsigned int sec_no, unsigned int num_of_sec) {
    asm("int $0x80\n\t"::"a"(4), "D"(sec_no), "S"(num_of_sec):);
}

void exit() {
    asm("int $0x80\n\t"::"a"(5):);
}