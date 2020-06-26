#include "libc.h"

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