#define KEYBOARD_IRQ 0x21
#define CLOCK_IRQ 0x20
#define UNDEFINE_IRQ 0x22
#define SYSCALL_IRQ 0x80
.section .text
.extern Welcoming
.extern Int22
.global _start
.extern SetInterupt
.extern KbHandler
.extern CcHandler
.extern SyscallHandler
.extern InitConsole
.extern save
.extern restart
.extern InitProcess
.extern InitTSS
.extern system_stack_addr

_start:
.code64
    lgdt gdt64desc
    push %rbp
    movq %rsp, %rbp

setint_8259:
    movb $0x11, %al     # ICW1
    out %al, $0x20  
    out %al, $0xa0

    movb $0x20, %al     # master ICW2
    out %al, $0x21
    
    movb $0x28, %al     # slave ICW2
    out %al, $0xa1

    movb $0x04, %al     # master ICW3
    out %al, $0x21

    movb $0x02, %al     # slave ICW3
    out %al, $0xa1

    movb $0x01, %al     # ICW4
    out %al, $0x21
    out %al, $0xa1

    movb $0xfc, %al
    out %al, $0x21      # 屏蔽中断

    movb $0xff, %al     # 屏蔽从8259的所有中断
    out %al, $0xa1
    
    # 初始化TSS，保证堆栈切换的正确运行
    movq $Tss_seg, %rdi
    movq $Tss_base, %rsi
    call InitTSS
    mov $0x20, %rax
    ltr %ax

    call InitClock
    call InitKeyboard
    call Init22
    call InitSyscall
    call InitConsole
    call InitProcess
    pop %rbp
spin:
    jmp spin

.global InitClock
.type	InitClock, @function
InitClock:
    movq $CLOCK_IRQ, %rdi
    movabsq $CcInt, %rsi
    call SetInterupt
    ret

.global InitKeyboard
.type	InitKeyboard, @function
InitKeyboard:
    movq $KEYBOARD_IRQ, %rdi
    movabsq $KbInt, %rsi
    call SetInterupt
    ret

.global InitSyscall
.type	InitSyscall, @function
InitSyscall:
    # movq $0x08080000, %rax
    # movq $0xc0000081, %rcx
    # wrmsr

    # movabsq $Syscall, %rax
    # movq $0xc0000082, %rcx 
    # wrmsr

    # movq $0x0, %rax
    # movq $0xc0000084, %rcx
    # wrmsr 
    movq $SYSCALL_IRQ, %rdi
    movabsq $Syscall, %rsi
    call SetInterupt
    ret
    ret

.global Init22
.type	Init22, @function
Init22:
    movq $UNDEFINE_IRQ, %rdi
    movabsq $Int22, %rsi
    call SetInterupt
    ret

.global Int22
.type	Int22, @function
Int22:
    call save
    movq %rax, %rdi
    call Int22Handler
    jmp restart

.global KbInt
.type	KbInt, @function
KbInt:
    call save
    movq %rax, %rdi
#   mov system_stack_addr, %rsp
    call KbHandler
    jmp restart

.global CcInt
.type	CcInt, @function
CcInt:
    call save
    movq %rax, %rdi
    call CcHandler
    jmp restart

.global Syscall
Syscall:
    call save
    movq %rax, %rdi
    call SyscallHandler
    jmp restart

.global ChangePageTable
ChangePageTable:
    movq $0x9000, %rax                           # Pointer to Final PML4 table
    movq %rax, %cr3                                 # Initialize CR3 with PML4 base.
    ret

temp_addr:
    .quad 0

kern_stack:
    .quad 0x20000000

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
    .byte 0xf2
    .byte 0
    .byte 0
    # CS DPL = 3
    .word 0
    .word 0
    .byte 0
    .byte 0xfa
    .byte 0xaf
    .byte 0
    # TSS Segment (Reserved to fill by kernel)
Tss_seg:
    .word 0x103
    .word 0x0
    .byte 0x0
    .byte 0x89
    .byte 0x00
    .byte 0x0
    .quad 0x0
    
Tss_base:
    .word 0
    .quad 0
gdt64desc:
    .word 0x40
    .quad gdt64
