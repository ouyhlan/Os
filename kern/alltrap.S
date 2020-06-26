# 每一个中断和系统调用都会进入的这里
.section .text
# struct trapframe *save()
.global save
save:
.code64
    # 记录调用者的返回地址
    pop %rbp

    # 保存所有的寄存器
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rsp    
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx
    push %rax

    movq %rsp, %rax

    # 恢复返回地址
    push %rbp
    lea 0x8(%rsp), %rbp
    ret

# void restart()
.global restart
restart:
    # EOI
    movb $0x20, %al
    out %al, $0x20
    
    # 恢复原来的寄存器
    pop %rax
    pop %rbx
    pop %rcx
    pop %rdx
    pop %rsi
    pop %rdi
    pop %rsp
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15
    pop %rbp
    iretq
