#include "mem.h"

asm(".code32\n\t");
void SetPageTable() {
    unsigned long long *pml4 = PML4_BASE;

    pml4[0] = PDPE_BASE | Uncached;

    unsigned long long *pdp = PDPE_BASE;
    pdp[0] = PDT_BASE | Uncached;

    unsigned long long *pdt = PDT_BASE;
    pdt[0] = PTE_KERNAL_BASE | Uncached;
    pdt[256] = PTE_KERNAL_BASE | Uncached;

    unsigned long long *pt_kern = PTE_KERNAL_BASE;
    /* Setting kernal for 2MB */
    for (int i = 0; i < 512; ++i) {
        pt_kern[i] = (i * Page_Size) | Uncached;
    }
    pt_kern[0xB8] |= Uncached; /* Setting display memory */

    /* 把所有空闲内存放到虚拟地址 = Kernal_Base + 物理地址 */
    for (int i = 1; i < Memory_Size / 0x200000; i += 1) {
        unsigned long long pt_addr = i * PT_Size;
        unsigned long long *pt_free_memory = pt_addr;
        
        pt_free_memory[0] = pt_addr | Uncached;
        for (int i = 1; i < 512; ++i) {
            pt_free_memory[i] = (pt_addr + i * Page_Size) | Unused_Uncached;  
        }
        pdt[256 + i] = pt_addr | Uncached; 
    }

    /* Setting IDT for long mode */
    unsigned long long *idt_addr = IDT_BASE;
    for (int i = 0; i < 1 << 8; ++i) {
        idt_addr[i * 2] = 0x00000e0000080000;
        idt_addr[i * 2 + 1] = 0;
    }
    
}