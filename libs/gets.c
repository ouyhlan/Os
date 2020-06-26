#include "libc.h"

void gets(char *buf) {
    asm("int $0x80\n\t"::"a"(3), "D"(buf):);
}