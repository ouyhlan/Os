#include "driver.h"
#define SECT_SIZE 512

int inp(int port) {
    int res;
    asm(   "mov %1, %%edx\n\t"
                "in %%dx, %%al\n\t"
                "movl %%eax, %0\n\t"
                :"=r"(res)
                :"r"(port)
                :"%rdx", "%rax"
    );
    return res;
}
void outp(int port, int info) {
    asm(    "movl %0, %%edx\n\t"
            "movl %1, %%eax\n\t"
            "out %%al, %%dx\n\n"
            :
            :"r"(port), "r"(info)
            :"%rax", "%rdx"
            );
}

void insl(short port, void *des, unsigned long long len) {
    asm(    "cld\n\t"
            "movq %2, %%rcx\n\t"
            "movq %1, %%rdi\n\t"
            "movw %0, %%dx\n\t"
            "rep insl\n\t"
            :
            :"r"(port), "r"(des), "r"(len)
            :
    );
}

/* 输入设置的中断向量号及对应的中断向量例程 */
void SetInterupt(unsigned short int_num, void *func) {
    __asm__("cli\n\t");                         /* 关闭中断 */
    InterruptGate *idt_addr = 0x100000;
    unsigned long long func_addr = (unsigned long long)func;
    idt_addr[int_num].reserved = 0x0;
    idt_addr[int_num].offset_high = func_addr >> 32;
    idt_addr[int_num].offset_mid = (func_addr >> 16) & 0xffff;
    idt_addr[int_num].offset_low = func_addr & 0xffff;
    idt_addr[int_num].selector = IDT_CS_SELECTOR;
    idt_addr[int_num].attr = IDT_PRESENT | IDT_SYSTEM_DPL | IDT_x86_64_TYPE;
    idt_addr[int_num].reserved = 0;
    __asm__("sti\n\t");
}

/* 设置键盘输入缓冲区 */
char inputbuf[32] = {0};

void KbHandler(struct TrapFrame *tf) {
    unsigned char scan_code = inp(0x60);
    
    /* 暂时不支持大写输入 */
    if (scan_code < 0x80) {
        char ch = normalmap[scan_code];
        kb_manager.buf[kb_manager.wpos] = ch;
        kb_manager.wpos = (kb_manager.wpos + 1) % BUFSIZE;
    }
}

static int cc_index = 0;
void CcHandler(struct TrapFrame *tf) {
    char *hot_wheels_shape = "|/\\";
    
    unsigned char *vga_addr = 0xb8000;
    
    /* 显示风火轮 */
    vga_addr[((24 * 80) + 79) * 2] = hot_wheels_shape[cc_index];
    vga_addr[((24 * 80) + 79) * 2 + 1] = 0x30;
    
    /* 更新风火轮状态 */
    cc_index = (cc_index + 1) % 3;
}

void ScrollUp() {
    CurPosition cur_pos = sys_GetCurPos();
    unsigned short *vga_addr = 0xb8000;     /* 两个字节为基本单位 */
    for (int i = 0; i < 24 * 80 - 1; ++i) {
        vga_addr[i] = vga_addr[i + 80];
    }
    for (int i = 24 * 80; i < 25 * 80; ++i) {
        vga_addr[i] = 0x3020;
    }
    cur_pos.y--;                            /* 光标随之上移一行 */                        
    sys_SetCurPos(cur_pos);
}

void InitConsole() {
    CurPosition init_pos = {0, 0};
    sys_SetCurPos(init_pos);
    
    unsigned char *vga_addr = 0xb8000;
    for (int i = 0; i < 25 * 80; ++i) {
        vga_addr[2 * i] = 0x20;               /* 设置青色背景 */
        vga_addr[2 * i + 1] = 0x30;
    }
}

/* 因为不采取中断提示方式，这里采用了轮询方式查看硬盘是否空闲 */
void WaitDisk() {
    while ((inp(0x1f7) & 0xC0) != 0x40)
        /* 等待硬盘空闲 */;
}

char sys_getchar() {
    char output;
    while (kb_manager.wpos == kb_manager.rpos) {
        /* Do nothing */;
    }
    output = kb_manager.buf[kb_manager.rpos];
    kb_manager.rpos = (kb_manager.rpos + 1) % BUFSIZE;
    sys_putc(output);
    return output;
}

void sys_gets(char *buf) {
    int buf_index = 0;
    for (char ch = sys_getchar(); ch != '\n'; ch = sys_getchar()) {
        buf[buf_index++] = ch;
    }
    sys_putc('\n');
    buf[buf_index] = '\0';
}

/* 显示一个字符 */
void sys_putc(char c) {
    unsigned char *vga_addr = 0xb8000;
    CurPosition cur_pos = sys_GetCurPos();
    if (c == '\n') {
        cur_pos.x = 0;                  /* 回车 */
        ++cur_pos.y;                    /* 换行 */
    }
    else {
        unsigned int ch_pos = (cur_pos.y * 80 + cur_pos.x) << 1;
        vga_addr[ch_pos] = c;
        vga_addr[ch_pos + 1] = 0x31;
        ++cur_pos.x;
    }
    sys_SetCurPos(cur_pos);
    
    /* 当当前显示超出25 * 80这个范围时,整体滚屏上移 */
    if (cur_pos.y * 80 + cur_pos.x >= 25 * 80) {
        ScrollUp();
    }
}

/* 显示一段字符串 */
void sys_puts(char *str) {
    int index = 0;
    while (str[index] != '\0') {
        sys_putc(str[index++]);
    }
}

/* 入口参数: dst:目的内存地址（线性地址） sec_no:在硬盘的扇区号 num_of_sec:要连续读取的扇区数 */
void readsect(void *dst, int sec_no, short num_of_sec) {
    // wait for disk to be ready
    WaitDisk();

    outp(0x1f2, num_of_sec);                    /* 设置读取扇区数 */
    outp(0x1f3, sec_no & 0xff);
    outp(0x1f4, (sec_no >> 8) & 0xff);
    outp(0x1f5, (sec_no >> 16) & 0xff);
    outp(0x1f6, ((sec_no >> 24) & 0xf) | 0xe0);
    outp(0x1f7, 0x20);                          /* 设置LBA模式 */

    /* 等待硬盘空闲 */
    WaitDisk();

    /* 一次读完全部到指定地址 */
    insl(0x1f0, dst, SECT_SIZE * num_of_sec / 4);
}

CurPosition sys_GetCurPos() {
    unsigned short temp = 0;
    CurPosition res;
    outp(0x3d4, 14);
    temp = (inp(0x3d5) & 0xff) << 8;     /* 光标位置高八位 */
    outp(0x3d4, 15);
    temp += inp(0x3d5);     /* 光标位置低八位 */
    res.x = temp % 80; 
    res.y = temp / 80;
    return res;
}

void sys_SetCurPos(CurPosition point_pos) {
    unsigned short pos = point_pos.y * 80 + point_pos.x;
    outp(0x3d4, 14);
    outp(0x3d5, (pos >> 8) & 0xff);     /* 设置光标高八位 */
    
    outp(0x3d4, 15);
    outp(0x3d5, pos & 0xff);            /* 设置光标低八位 */

}

uint64_t user_program_stack = 0x0;
/* 指明要加载的用户程序扇区号和所需扇区数 */
void sys_exec(unsigned int sec_no, unsigned int num_of_sec) {
    void *user_code_addr = 0x400000;
    readsect(user_code_addr, sec_no, num_of_sec);   /* 加载用户程序到内存里 */
    __asm__("movq %%rsp, %0":"=r"(user_program_stack)::);
    __asm__("sti\n\tcall %0\n\t"::"r"(user_code_addr):);
}

void sys_exit() {
    user_program_stack -= 8;
    asm("movq %0, %%rsp\n\tret\n\t"::"r"(user_program_stack):);
}

void SyscallHandler(struct TrapFrame *tf) {
    switch (tf->rax) {
        case 0: sys_putc((char)tf->rdi) ;break;
        case 1: sys_puts((char*)tf->rdi) ;break;
        case 2: __asm__("sti"); tf->rax = sys_getchar(); __asm__("cli"); break;
        case 3: __asm__("sti"); sys_gets((char*)tf->rdi); __asm__("cli"); break;
        case 4: sys_exec((unsigned int)tf->rdi, (unsigned int)tf->rsi); break;
        case 5: sys_exit(); break;
    }
}

void Int22Handler(struct TrapFrame *tf) {
    CurPosition pos = {53, 18};
    sys_SetCurPos(pos);
    sys_puts("Int 0x22");
}