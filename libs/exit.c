#include "libc.h"

void exit() {
    asm("int $0x80\n\t"::"a"(5):);
}