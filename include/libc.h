#ifndef __LIBC_H__
#define __LIBC_H__

#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long long

typedef __builtin_va_list va_list;
#define va_start(v,l)	__builtin_va_start(v,l)
#define va_end(v)	__builtin_va_end(v)
#define va_arg(v,l)	__builtin_va_arg(v,l)

int strncmp(char *str1, char *str2, unsigned int n);
int strlen(char *str);
void printf(char *str, ...);
void scanf(char *, ...);
char getchar();
void gets(char *buf);
void putc(char c);
void puts(char *str);
void exec(unsigned int sec_no, unsigned int num_of_sec);
void exit();
void multiexec(uint64_t proc_num, uint32_t *sec_no, uint32_t *num_of_sec);
#endif