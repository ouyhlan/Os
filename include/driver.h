#ifndef __DRIVER_H__
#define __DRIVER_H__
#define Max_Pro_Num 10
#define SHELL_SEC 32
#define SHELL_NUM 10
#define SECTSIZE 512
#define NO 0
#define KEYBOARD_IRQ 0x21
#define CLOCK_IRQ 0x20
#define IDT_CS_SELECTOR 0x8;
#define IDT_x86_64_TYPE 0x0e00;
#define IDT_SYSTEM_DPL  0x6000
#define IDT_PRESENT     0x8000
#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long long
#define pde_t uint64_t
#define BUFSIZE 512
#pragma pack(1)

typedef struct InterruptGate {
    uint16_t offset_low;
    uint16_t selector;
    uint16_t attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
} InterruptGate;


typedef struct CurPosition {
    int x, y;
} CurPosition;

struct TrapFrame {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss; 
};

struct {
    uint8_t buf[BUFSIZE];
    uint32_t rpos;
    uint32_t wpos;
} kb_manager;

enum state{UNUSED = 0 ,READY = 1, RUNNING = 2};
struct Proc {
    //uint64_t *k_stack;  /* 进程在系统进程栈起点 */
    struct TrapFrame *tf;
    pde_t *pd;
    int proc_state;
    uint64_t proc_id;
    
};

// 汇编函数
int inb(int port);
void outb(int port, int info);
void insl(short port, void *des, unsigned long long len);

// 硬件驱动函数
void WaitDisk();
void readsect(void *dst, int secno, short num_of_sec);
void KbHandler();
void SetInterupt(unsigned short int_num, void *func);

// 系统调用函数
void sys_putc(char c);
void sys_puts(char *str);
char sys_getchar();
void sys_gets(char *buf);
void sys_setreturnpos();
void sys_multiexec(uint64_t proc_num, uint32_t *sec_no, uint32_t *num_of_sec);

// 进程管理函数
void InitProcess();
int CreateProcess(int sec_no, short num_of_sec);
void DestroyProcess(uint64_t pid);
void Scheduler();
void ChangePd(uint64_t new_pd_addr);
void ChangePageTable();

// 虚拟内存管理函数
uint64_t Kmalloc();
void Kmfree(uint64_t mem_addr);

CurPosition sys_GetCurPos();
void sys_SetCurPos(CurPosition point_pos);
void Int22Handler(struct TrapFrame *tf);

/* 键盘映射 */
static char normalmap[256] = {
    NO,   0x1B, '1',  '2',  '3',  '4',  '5',  '6',  // 0x00
    '7',  '8',  '9',  '0',  '-',  '=',  '\b', '\t',
    'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  // 0x10
    'o',  'p',  '[',  ']',  '\n', NO,   'a',  's',
    'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  // 0x20
    '\'', '`',  NO,   '\\', 'z',  'x',  'c',  'v',
    'b',  'n',  'm',  ',',  '.',  '/',  NO,   '*',  // 0x30
    NO,   ' ',  NO,   NO,   NO,   NO,   NO,   NO,
    NO,   NO,   NO,   NO,   NO,   NO,   NO,   '7',  // 0x40
    '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
    '2',  '3',  '0',  '.',  NO,   NO,   NO,   NO   // 0x50
};

static char shiftmap[256] = {
    NO,   033,  '!',  '@',  '#',  '$',  '%',  '^',  // 0x00
    '&',  '*',  '(',  ')',  '_',  '+',  '\b', '\t',
    'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',  // 0x10
    'O',  'P',  '{',  '}',  '\n', NO,   'A',  'S',
    'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',  // 0x20
    '"',  '~',  NO,   '|',  'Z',  'X',  'C',  'V',
    'B',  'N',  'M',  '<',  '>',  '?',  NO,   '*',  // 0x30
    NO,   ' ',  NO,   NO,   NO,   NO,   NO,   NO,
    NO,   NO,   NO,   NO,   NO,   NO,   NO,   '7',  // 0x40
    '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
    '2',  '3',  '0',  '.',  NO,   NO,   NO,   NO   // 0x50
};

#endif