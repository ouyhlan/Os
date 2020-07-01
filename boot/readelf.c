#include <elf.h>
#define Under_Addr(X) (((X) >> 12) << 12)

void MemCopy(void *src, void *dest, unsigned long long n) {
    for (int i = 0; i < n; ++i) {
        ((char*)dest)[i] = ((char *)src)[i];
    }
}

void ReadElf(void *file_addr) {
    Elf64_Ehdr *elf_header = file_addr;
    for (int i = 0; i < elf_header->e_phnum; ++i) {
        Elf64_Phdr *program_header = (Elf64_Phdr *)file_addr + elf_header->e_phoff;
        if (program_header->p_filesz != 0)
            MemCopy(file_addr, program_header->p_offset, program_header->p_filesz);       
    }
}