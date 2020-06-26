#include "libc.h"

void exec(unsigned int sec_no, unsigned int num_of_sec) {
    asm("int $0x80\n\t"::"a"(4), "D"(sec_no), "S"(num_of_sec):);
}