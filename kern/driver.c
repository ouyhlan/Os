#include "1.h"
#define SECT_SIZE 512
#define EOI()   \
    __asm__("movb $0x20, %al\n\t"); \
    __asm__("out %al, $0x20\n\t");  

/* Compare two string 
** 0 for same, -1 for not the same  
*/
int StrCmp(char *str1, char *str2, unsigned int n) {
    for (int i = 0; i < n; ++i) {
        if (str1[i] != str2[i])
            return -1;
    }
    return 0;
}

int StrLen(char *str) {
    int count = 0;
    while (str[count++] != '\0') {
    }
    return count - 1;
}

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

#pragma pack(1)
typedef struct InterruptGate {
    uint16_t offset_low;
    uint16_t selector;
    uint16_t attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
} InterruptGate;

#define IDT_CS_SELECTOR 0x8;
#define IDT_x86_64_TYPE 0x0e00;
#define IDT_SYSTEM_DPL  0x0
#define IDT_PRESENT     0x8000

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

/* 设置键盘输入缓冲区 */
char inputbuf[32] = {0};

#define BUFSIZE 512

struct {
    uint8_t buf[BUFSIZE];
    uint32_t rpos;
    uint32_t wpos;
} kb_manager;

__attribute__ ((interrupt))
void KbHandler(struct interrupt_frame* frame) {
    unsigned char scan_code = inp(0x60);
    
    /* 暂时不支持大写输入 */
    if (scan_code < 0x80) {
        char ch = normalmap[scan_code];
        kb_manager.buf[kb_manager.wpos] = ch;
        kb_manager.wpos = (kb_manager.wpos + 1) % BUFSIZE;
    }
    EOI();
}

void InitKeyboard() {
    SetInterupt(KEYBOARD_IRQ, KbHandler);
    kb_manager.rpos = kb_manager.wpos = 0;
}

char getchar() {
    char output;
    while (kb_manager.wpos == kb_manager.rpos) {
        /* Do nothing */;
    }
    output = kb_manager.buf[kb_manager.rpos];
    kb_manager.rpos = (kb_manager.rpos + 1) % BUFSIZE;
    putc(output);
    return output;
}

void gets(char *buf) {
    int buf_index = 0;
    for (char ch = getchar(); ch != '\n'; ch = getchar()) {
        buf[buf_index++] = ch;
    }
    putc('\n');
    buf[buf_index] = '\0';
}

/* 暂时不提供异常处理 */
void scanf(char *str, ...) {
    va_list ap;
    int str_len = StrLen(str), arg_len = 0;

    /* 计算输入的参数 */
    for (int i = 0; i < str_len; ++i) {
        if (str[i] == '%') {
            arg_len++;
        }
    }

    va_start(ap, arg_len);
    for (int i = 0; i < str_len; ++i) {
        if (str[i] == '%') {
            char ch;
            while ((ch = getchar()) == ' ' || ch == '\n') {  /* 忽略开头的空格 */
                /* do nothing */;
            }

            /* 处理数字输出 */
            if (str[i + 1] == 'd') {
                /* 处理输入 */
                int dig_pos = 0, num_res = 0;
                char digit_buf[9] = {0};   /* 一般int只有9位 */
                for (ch; (ch >= '0' && ch <= '9'); ch = getchar()) {
                    digit_buf[dig_pos++] = ch;
                }

                /* str2int */
                for (int j = 0; j < dig_pos; ++j) {
                    num_res = num_res * 10 + (digit_buf[j] - '0');
                }

                /* 赋值语句 */
                int *scanf_num = va_arg(ap, int *);
                *scanf_num = num_res;
            }
            else if (str[i + 1] == 's') {
                char *scanf_str = va_arg(ap, char*);
                int str_pos = 0;
                for (ch; ch != ' ' && ch != '\n'; ch = getchar()) {
                    scanf_str[str_pos++] = ch;
                }
                scanf_str[str_pos] = '\0';
            }
            ++i;    /* 跳过%后面的字符 */
        }
    }
}

static int cc_index = 0;
__attribute__ ((interrupt))
void CcHandler(struct interrupt_frame* frame) {
    char *hot_wheels_shape = "|/\\";
    
    unsigned char *vga_addr = 0xb8000;
    
    /* 显示风火轮 */
    vga_addr[((24 * 80) + 79) * 2] = hot_wheels_shape[cc_index];
    vga_addr[((24 * 80) + 79) * 2 + 1] = 0x30;
    
    /* 更新风火轮状态 */
    cc_index = (cc_index + 1) % 3;

    EOI();
}

/* 时钟中断初始化 */
void InitClock() {
    SetInterupt(CLOCK_IRQ, CcHandler);
}

CurPosition GetCurPos() {
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

void SetCurPos(CurPosition point_pos) {
    unsigned short pos = point_pos.y * 80 + point_pos.x;
    outp(0x3d4, 14);
    outp(0x3d5, (pos >> 8) & 0xff);     /* 设置光标高八位 */
    
    outp(0x3d4, 15);
    outp(0x3d5, pos & 0xff);            /* 设置光标低八位 */

}

void ScrollUp() {
    CurPosition cur_pos = GetCurPos();
    unsigned short *vga_addr = 0xb8000;     /* 两个字节为基本单位 */
    for (int i = 0; i < 24 * 80 - 1; ++i) {
        vga_addr[i] = vga_addr[i + 80];
    }
    for (int i = 24 * 80; i < 25 * 80; ++i) {
        vga_addr[i] = 0x3020;
    }
    cur_pos.y--;                            /* 光标随之上移一行 */                        
    SetCurPos(cur_pos);
}


/* 仅支持非负整数、字符串输出
** 暂时不支持负数、浮点数输出
** 还没有错误处理！！！
*/
void printf(char *str, ...) {
    va_list ap;
    int str_len = StrLen(str), arg_len = 0;

    for (int i = 0; i < str_len; ++i) {
        if (str[i] == '%') {
            arg_len++;
        }
    }

    va_start(ap, arg_len);

    for (int i = 0; i < str_len; ++i) {
        if (str[i] == '%') {
            /* 处理数字输出 */
            if (str[i + 1] == 'd') {
                int digit_temp = va_arg(ap, int); /* 读取参数 */
                /* int2str */
                int dig_pos = 0;
                char digit_buf[9] = {0};   /* 一般int只有9位 */
                while (digit_temp > 0) {
                    digit_buf[dig_pos++] = digit_temp % 10 + '0';
                    digit_temp /= 10;
                }
                for (int j = dig_pos - 1; j >= 0; --j) {
                    putc(digit_buf[j]);
                }
            }
            else if (str[i + 1] == 's') {
                char *print_str = va_arg(ap, char*);
                int index = 0;
                while (print_str[index] != '\0') {
                    putc(print_str[index++]);
                }
            }
            ++i;    /* 跳过%后面的字符 */
        }
        else {
            putc(str[i]);
        }
    }
    va_end(ap);
}

/* 显示一个字符 */
void putc(char c) {
    unsigned char *vga_addr = 0xb8000;
    CurPosition cur_pos = GetCurPos();
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
    SetCurPos(cur_pos);
    
    /* 当当前显示超出25 * 80这个范围时,整体滚屏上移 */
    if (cur_pos.y * 80 + cur_pos.x >= 25 * 80) {
        ScrollUp();
    }
}

/* 显示一段字符串 */
void puts(char *str) {
    int index = 0;
    while (str[index] != '\0') {
        putc(str[index++]);
    }
    putc('\n');
}


/* 因为不采取中断提示方式，这里采用了轮询方式查看硬盘是否空闲 */
void WaitDisk() {
    while ((inp(0x1f7) & 0xC0) != 0x40)
        /* 等待硬盘空闲 */;
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

