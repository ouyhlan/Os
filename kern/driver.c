#include "driver.h"
#include "mem.h"
#define SECT_SIZE 512


uint64_t *tss;  /* 设置TSS指针 */
int running_pid = 0;    /* 当前正在运行的进程号 */
struct Proc process_table[Max_Pro_Num]; /* 进程表 */

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
    InterruptGate *idt_addr = 0x2000d000;
    unsigned long long func_addr = (unsigned long long)func;
    idt_addr[int_num].reserved = 0x0;
    idt_addr[int_num].offset_high = func_addr >> 32;
    idt_addr[int_num].offset_mid = (func_addr >> 16) & 0xffff;
    idt_addr[int_num].offset_low = func_addr & 0xffff;
    idt_addr[int_num].selector = IDT_CS_SELECTOR;
    idt_addr[int_num].attr = IDT_PRESENT | IDT_SYSTEM_DPL | IDT_x86_64_TYPE;
    idt_addr[int_num].reserved = 0;
    //__asm__("sti\n\t");
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


void CcHandler(struct TrapFrame *tf) {
    /* 无敌风火轮 */
    static int cc_index = 0;

    char *hot_wheels_shape = "|/\\"; 
    unsigned char *vga_addr = 0x200b8000;
    
    /* 显示风火轮 */
    vga_addr[((24 * 80) + 79) * 2] = hot_wheels_shape[cc_index];
    vga_addr[((24 * 80) + 79) * 2 + 1] = 0x30;
    
    /* 更新风火轮状态 */
    cc_index = (cc_index + 1) % 3;

    Scheduler();
}

void ScrollUp() {
    CurPosition cur_pos = sys_GetCurPos();
    unsigned short *vga_addr = 0x200b8000;     /* 两个字节为基本单位 */
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
    kb_manager.rpos = kb_manager.wpos = 0;
    unsigned char *vga_addr = 0x200b8000;
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
        if (ch == '\b') {
            if (buf_index > 0)
                buf[--buf_index] = '\0';
        }
        else {
            buf[buf_index++] = ch;
        }
    }
    sys_putc('\n');
    buf[buf_index] = '\0';
}

/* 显示一个字符 */
void sys_putc(char c) {
    unsigned char *vga_addr = 0x200b8000;
    CurPosition cur_pos = sys_GetCurPos();
    if (c == '\n') {
        cur_pos.x = 0;                  /* 回车 */
        ++cur_pos.y;                    /* 换行 */
    }
    else if (c == '\b') {
        if (cur_pos.x > 0)
            --cur_pos.x;
        unsigned int ch_pos = (cur_pos.y * 80 + cur_pos.x) << 1;
        vga_addr[ch_pos] = ' ';
        vga_addr[ch_pos + 1] = 0x31;
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

/* 指明要加载的用户程序扇区号和所需扇区数 */
void sys_exec(struct TrapFrame *tf, unsigned int sec_no, unsigned int num_of_sec) {
    /* 为了显示效果，先清屏 */
    unsigned char *vga_addr = 0x200b8000;
    for (int i = 0; i < 25 * 80; ++i) {
        vga_addr[2 * i] = 0x20;               /* 设置青色背景 */
        vga_addr[2 * i + 1] = 0x30;
    }

    /* 创建进程 */
    CreateProcess(sec_no, num_of_sec);
    Scheduler();
}

void sys_exit() {
    /* 销毁进程 */
    DestroyProcess(running_pid);
    Scheduler();
}

/* 设置回归光标在最后一行，便于显示 */
void sys_setreturnpos() {
    unsigned short pos = 24 * 80 + 0;
    outp(0x3d4, 14);
    outp(0x3d5, (pos >> 8) & 0xff);     /* 设置光标高八位 */
    
    outp(0x3d4, 15);
    outp(0x3d5, pos & 0xff);            /* 设置光标低八位 */

}

void sys_multiexec(uint64_t proc_num, uint32_t *sec_no, uint32_t *num_of_sec) {
    /* 为了显示效果，先清屏 */
    unsigned char *vga_addr = 0x200b8000;
    for (int i = 0; i < 25 * 80; ++i) {
        vga_addr[2 * i] = 0x20;               /* 设置青色背景 */
        vga_addr[2 * i + 1] = 0x30;
    }

    /* 创建进程 */
    for (int i = 0; i < proc_num; ++i) {
        CreateProcess(sec_no[i], num_of_sec[i]);
    }
    Scheduler();
}

void SyscallHandler(struct TrapFrame *tf) {
    switch (tf->rax) {
        case 0: sys_putc((char)tf->rdi) ;break;
        case 1: sys_puts((char*)tf->rdi) ;break;
        case 2: __asm__("sti"); tf->rax = sys_getchar(); __asm__("cli"); break;
        case 3: __asm__("sti"); sys_gets((char*)tf->rdi); __asm__("cli"); break;
        case 4: sys_exec(tf, (unsigned int)tf->rdi, (unsigned int)tf->rsi); break;
        case 5: sys_exit(); break;
        case 6: sys_setreturnpos(); break;
        case 7: sys_multiexec(tf->rdi, tf->rsi, tf->rdx);break;
    }
}

void InitTSS(uint64_t *tss_seg, uint64_t tss_base_addr) {
    tss_seg[0] |= ((tss_base_addr & 0xffffff) << 16) | (((tss_base_addr >> 24) & 0xff) << 56);
    tss_seg[1] |= ((tss_base_addr >> 32) & 0xffffffff);
    tss = tss_base_addr + 4;
}

/* 初始化第一个进程 pid = 0 */
void InitProcess() {
    for (int i = 0; i < Max_Pro_Num; ++i) {
        process_table[i].proc_state = 0;
        process_table[i].tf = TrapFrameMem + i * 100 * sizeof(struct TrapFrame);
    }

    process_table[0].proc_id = 0;
    uint64_t pd_addr = Kmalloc();
    process_table[0].pd = PHY2VIR(pd_addr);

    /* 申请页表 */
    uint64_t pt_addr = Kmalloc();
    process_table[0].pd[0] = (pt_addr | Uncached);
    uint64_t *pt = PHY2VIR(pt_addr);

    /* 申请页框,并加载进程到内存0x0 */
    pt[0] = Kmalloc() | Uncached;
    pt[1] = Kmalloc() | Uncached;
    pt[511] = Kmalloc() | Uncached;
    readsect(PHY2VIR(pt[0] - Uncached), SHELL_SEC, SHELL_NUM);
    
    process_table[0].tf->cs = 0x1b;
    process_table[0].tf->rip = 0x0;
    process_table[0].tf->rflags = 0x3202;
    process_table[0].tf->ss = 0x13;
    process_table[0].tf->rsp = 0x1fffff;
    running_pid = 0;

    /* 更换Page Table Directory */ 
    ChangePd(pd_addr);
    
    /* 跳转运行 */
    void *shell_code_addr = 0x0;
    process_table[0].proc_state = RUNNING;
    tss[0] = (uint64_t)(process_table[0].tf) + sizeof(struct TrapFrame);
    __asm__("mov %0, %%rsp\n\t"::"r"(process_table[0].tf):);
    __asm__("jmp restart\n\t");
}

/* 创建新的进程 sec_no:在硬盘的扇区号 num_of_sec:要连续读取的扇区数,返回进程号 */
int CreateProcess(int sec_no, short num_of_sec) {
    int i;
    for (i = 0; i < Max_Pro_Num; ++i) {
        if (process_table[i].proc_state == UNUSED) 
            break;
    }
    
    /* 设置进程表 */
    process_table[i].proc_id = i;
    uint64_t pd_addr = Kmalloc();
    process_table[i].pd = PHY2VIR(pd_addr);

    /* 为进程申请页表 */
    uint64_t pt_addr = Kmalloc();
    process_table[i].pd[0] = (pt_addr | Uncached);
    uint64_t *pt = PHY2VIR(pt_addr);

    /* 申请页框给内存0x0 */
    pt[0] = Kmalloc() | Uncached;
    pt[1] = Kmalloc() | Uncached;
    pt[511] = Kmalloc() | Uncached;
    readsect((void*)(PHY2VIR(pt[0] - Uncached)), sec_no, num_of_sec);
    
    /* 设置进程上下文 */
    process_table[i].tf->cs = 0x1b;
    process_table[i].tf->rip = 0x0;
    process_table[i].tf->rflags = 0x3202;
    process_table[i].tf->ss = 0x13;
    process_table[i].tf->rsp = 0x1fffff;
    process_table[i].proc_state = READY;
    return i;
}

void DestroyProcess(uint64_t pid) {
    /* 设置进程表 */
    process_table[pid].proc_state = UNUSED;
    pde_t pd_addr = (uint64_t)process_table[pid].pd;
    uint64_t pt_addr = process_table[pid].pd[0] - Uncached;
    uint64_t *pt = PHY2VIR(pt_addr);
    
    Kmfree(pt[0] - Uncached);
    Kmfree(pt[1] - Uncached);
    Kmfree(pt[511] - Uncached);
    Kmfree(pt_addr);
    Kmfree(pd_addr);
}

void Scheduler() {
    int count = 0;
    int ready_process[Max_Pro_Num];   
    for (int i = (running_pid + 1) % Max_Pro_Num; i != running_pid; i = (i + 1) % Max_Pro_Num) {
        if (process_table[i].proc_state == READY && i != 0) {
            ready_process[count++] = i;
        }
    }

    ready_process[count] = 0;
    
    if (process_table[running_pid].proc_state == RUNNING) {
        if (ready_process[0] == 0)
            return;
        process_table[running_pid].proc_state = READY;
    }

    int i = ready_process[0];
    process_table[i].proc_state = RUNNING;
    running_pid = i;
    
    //ChangeKernalPd();
    ChangePd(VIR2PHY((uint64_t)(process_table[i].pd)));
    tss[0] = (uint64_t)(process_table[i].tf) + sizeof(struct TrapFrame);
    asm("mov %0, %%rsp\n\tjmp restart\n\t"::"r"(process_table[i].tf):);

}

/* 更换Page Table Directory,传入参数为物理地址 */
void ChangePd(uint64_t new_pd_addr) {
    uint64_t *pdp = PHY2VIR(PDPE_BASE);
    pde_t *new_pd = PHY2VIR(new_pd_addr);
    pde_t *pdt = PHY2VIR(PDT_BASE);

    for (int i = 256; i < 512; ++i)
        new_pd[i] = pdt[i];
    pdp[0] = new_pd_addr | Uncached;
    
    /* 更新TLB */
    ChangePageTable();
}

/* 请求新的页框,返回的是空闲的物理页框号 */
uint64_t Kmalloc() {
    for (uint64_t i = FreeMem; i < Memory_Size; i += Page_Size) {
        /* 每个页表第0项跳过 */
        if (i % FreeMem == 0) 
            continue;
        uint64_t virt_addr = PHY2VIR(i);
        uint64_t *pt_addr = PT_Addr(virt_addr);
        if (!(pt_addr[PT_Index(virt_addr)] & PRESENTED)) {
            pt_addr[PT_Index(virt_addr)] |= PRESENTED;
            return i;
        }
    }
}

/* 释放输入的物理页框号 */
void Kmfree(uint64_t mem_addr) {
    uint64_t virt_addr = PHY2VIR(mem_addr);
    uint64_t *pt_addr = PT_Addr(virt_addr);

    /* 将页表里该项置为Unpresented */
    pt_addr[PT_Index(virt_addr)] -= 1; 
}

void Int22Handler(struct TrapFrame *tf) {
    CurPosition pos = {53, 18};
    sys_SetCurPos(pos);
    sys_puts("Int 0x22");
}