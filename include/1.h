#ifndef __ONE_H__
#define __ONE_H__
#define SECTSIZE 512
#define NO 0
#define KEYBOARD_IRQ 0x21
#define CLOCK_IRQ 0x20
#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long long

typedef __builtin_va_list va_list;
#define va_start(v,l)	__builtin_va_start(v,l)
#define va_end(v)	__builtin_va_end(v)
#define va_arg(v,l)	__builtin_va_arg(v,l)

int inb(int port);
void outb(int port, int info);
void insl(short port, void *des, unsigned long long len);
typedef struct CurPosition {
    int x, y;
} CurPosition;
CurPosition GetCurPos();
void SetCurPos(CurPosition point_pos);
void putc(char c);
void puts(char *str);
void WaitDisk();
void readsect(void *dst, int secno, short num_of_sec);
char getchar();
void gets(char *buf);
void KbHandler();
void SetInterupt(unsigned short int_num, void *func);
void SetOuch();
void InitKeyboard();
int StrCmp(char *str1, char *str2, unsigned int n);
int StrLen(char *str);
void printf(char *str, ...);
void scanf(char *, ...);
struct interrupt_frame{
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t ss;
};
#endif