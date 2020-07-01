# 每一个中断和系统调用都会进入的这里
.section .text
# struct trapframe *save()
.global save
.extern system_stack_addr

save:
.code64
    # 记录调用者的返回地址
    pop temp_addr

    # 保存所有的寄存器
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rbp    
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx
    push %rax

    movq %rsp, %rax

    # 恢复返回地址
    push temp_addr
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
    pop %rbp
    pop %r8
    pop %r9
    pop %r10
    pop %r11
    pop %r12
    pop %r13
    pop %r14
    pop %r15
#    pop %rsp
    iretq
temp_addr:
    .quad 0