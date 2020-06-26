#include "libc.h"

char getchar() {
    char res;
    asm("int $0x80\n\t":"=a"(res):"a"(2):);
    return res;
}