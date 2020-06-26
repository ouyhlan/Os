#include "libc.h"

/* 显示一个字符 */
void putc(char c) {
    asm("int $0x80\n\t"::"a"(0), "D"(c):);
}