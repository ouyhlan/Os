.set PML4_BASE,             0x100000                # pml-4 base address
.set LONG_MODE_CSEG,        0x8
.set LONG_MODE_SMEM,        0xffffffffffffffff          
.set LONG_MODE_SYSM,        0xffff800000000000
.set KERNEL_SEC,            0x5
.set NUM_KERNEL_SEC,        0x1b                    # read 16 secs


# start address should be 0:7e00, in protected mode, the beginning address of the third bootloader
.section .text
.extern ReadSect
.globl _start
_start:
    .code32
    #
    # Enable the 64-bit page-translation-table entries by
    # setting CR4.PAE=1 (this is _required_ before activating
    # long mode). Paging is not enabled until after long mode
    # is enabled.
    #
    movl %cr4, %eax
    bts $0x5, %eax
    movl %eax, %cr4
    #
    # Create the long-mode page tables, and initialize the
    # 64-bit CR3 (page-table base address) to point to the base
    # of the PML4 page table. The PML4 page table must be located
    # below 4 Gbytes because only 32 bits of CR3 are loaded when
    # the processor is not in 64-bit mode.
    #
    movl $PML4_BASE, %eax                           # Pointer to PML4 table (<4GB).
    movl %eax, %cr3                                 # Initialize CR3 with PML4 base.
    #
    # Enable long mode (set EFER.LME=1).
    #
    mov $0x0c0000080, %ecx                          # EFER MSR number.
    rdmsr                                           # Read EFER.
    bts $0x8, %eax                                  # Set LME=1.
    wrmsr                                           # Write EFER.
    #
    # Enable paging to activate long mode (set CR0.PG=1)
    #
    movl %cr0, %eax                                 # Read CR0.
    orl $0x80000000, %eax                           # Set PG=1.
    movl %eax, %cr0                                 # Write CR0.
    #
    # At this point, we are in 16-bit compatibility mode
    # ( LMA=1, CS.L=0, CS.D=0 ).
    lgdt gdt64desc
    lidt idt64desc
start64_linear:
    ljmp $LONG_MODE_CSEG, $longseg

.code64                                             # Assemble for 64-bit mode
longseg:
    movq $PML4_BASE, %rax                           # Pointer to Final PML4 table
    movq %rax, %cr3                                 # Initialize CR3 with PML4 base.
    movq $LONG_MODE_SMEM, %rsp                      # Setting Stack

    # Loading Kernel Code in
    movq $LONG_MODE_SYSM, %rdi
    movq $KERNEL_SEC, %rsi
    movq $NUM_KERNEL_SEC, %rdx
    call ReadSect

    movq $LONG_MODE_SYSM, %r9
    movq $gdt64, %rdi                               # Give the address of GDT to the kernal
    jmp %r9

spin:
    jmp spin

gdt64:
    # NULL
    .word 0xffff
    .word 0
    .byte 0
    .byte 0
    .byte 1
    .byte 0
    # Code
gdt64Code:
    .word 0
    .word 0
    .byte 0
    .byte 0x9a
    .byte 0xaf
    .byte 0
    # Data
    .word 0
    .word 0
    .byte 0
    .byte 0x92
    .byte 0
    .byte 0
    # TSS Segment (Reserved to fill by kernel)
    .quad 0
    .quad 0
gdt64desc:
    .word 0x30
    .quad gdt64
idt64desc:
    .word 0x8000
    .quad 0x100000
