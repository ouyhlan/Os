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
_start:
.code64
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
    
    call InitClock
    call InitKeyboard
    call Init22
    call InitSyscall
    call InitConsole
    sti
    call Welcoming
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
    push %rbp
    call save
    movq %rax, %rdi
    call Int22Handler
    jmp restart

.global KbInt
.type	KbInt, @function
KbInt:
    push %rbp
    call save
    movq %rax, %rdi
    call KbHandler
    jmp restart

.global CcInt
.type	CcInt, @function
CcInt:
    push %rbp
    call save
    movq %rax, %rdi
    call CcHandler
    jmp restart

.global Syscall
Syscall:
    push %rbp
    call save
    movq %rax, %rdi
    call SyscallHandler
    cli
    jmp restart