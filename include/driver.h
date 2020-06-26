#ifndef __DRIVER_H__
#define __DRIVER_H__
#define SECTSIZE 512
#define NO 0
#define KEYBOARD_IRQ 0x21
#define CLOCK_IRQ 0x20
#define IDT_CS_SELECTOR 0x8;
#define IDT_x86_64_TYPE 0x0e00;
#define IDT_SYSTEM_DPL  0x0
#define IDT_PRESENT     0x8000
#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long long
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
    uint64_t rsp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rbp;
};

struct {
    uint8_t buf[BUFSIZE];
    uint32_t rpos;
    uint32_t wpos;
} kb_manager;

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

CurPosition sys_GetCurPos();
void sys_SetCurPos(CurPosition point_pos);

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