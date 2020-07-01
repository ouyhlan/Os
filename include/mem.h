#ifndef __MEM_H__
#define __MEM_H__

#define Memory_Size 0x2000000
#define PML4_BASE 0x9000
#define PDPE_BASE 0xa000
#define PDT_BASE 0xb000
#define PTE_KERNAL_BASE 0xc000
#define IDT_BASE 0xd000
#define Cached 0x27
#define Uncached 0x2f
#define Page_Size 0x1000 
#define PT_Size 0x200000
#define Kernal_Base 0x20000000
#define Unused_Uncached 0x2e
// only use by kernal
#define FreeMem 0x200000
#define TrapFrameMem 0x201a0000
#define PT_Addr(X) ((X) >> 21) << 21
#define PT_Index(X) ((X) >> 12) & 0x1ff
#define PHY2VIR(X) ((X) | Kernal_Base)
#define VIR2PHY(X) ((X) - Kernal_Base)
#define PRESENTED 0x1
#endif