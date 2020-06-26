#include "libc.h"

/* 显示一段字符串 */
void puts(char *str) {
    asm("int $0x80\n\t"::"a"(1), "D"(str):);
    putc('\n');
}