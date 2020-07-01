#include "libc.h"
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
            while ((ch = getchar()) == ' ' || ch == '\n' || ch == '\b') {  /* 忽略开头的空格 */
                /* do nothing */;
            }

            /* 处理数字输出 */
            if (str[i + 1] == 'd') {
                /* 处理输入 */
                int dig_pos = 0, num_res = 0;
                char digit_buf[9] = {0};   /* 一般int只有9位 */
                for (ch; (ch >= '0' && ch <= '9') || ch == '\b'; ch = getchar()) {
                    if (ch == '\b') {
                        if (dig_pos > 0)
                            digit_buf[--dig_pos] = '\0';
                    }
                    else {
                        digit_buf[dig_pos++] = ch;
                    }
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
                    if (ch == '\b') {
                        if (str_pos > 0)
                            scanf_str[--str_pos] = '\0';
                    }
                    else {
                        scanf_str[str_pos++] = ch;
                    }
                }
                scanf_str[str_pos] = '\0';
            }
            ++i;    /* 跳过%后面的字符 */
        }
    }
}