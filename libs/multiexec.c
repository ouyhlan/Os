#include "libc.h"

void multiexec(uint64_t proc_num, uint32_t *sec_no, uint32_t *num_of_sec) {
    asm("int $0x80\n\t"::"a"(7), "D"(proc_num), "S"(sec_no), "d"(num_of_sec):);
}