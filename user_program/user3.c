#include "libc.h"
#define Lmax 0
#define Rmax 39
#define Umax 13
#define Dmax 24
#define Dn_Rt 1
#define Up_Rt 2
#define Up_Lt 3
#define Dn_Lt 4
#define delay 5000
#define ddelay 580

char *vga_addr = 0x200b8000;
char ch_disp = 'A';         /* 要显示的字符 */
short x = 16;       /* 行号 */
short y = 1;       /* 列号 */
unsigned int rdul = Dn_Rt;
char bg_color = 0x50;
char ft_color = 0x0;

void ShowChar();
void DownRight();
void DownLeft();
void UpRight();
void UpLeft();
void SetReturnPos();

int _start() {
    /* 清屏 */
    for (int i = Umax; i <= Dmax; ++i) {
        for (int j = Lmax; j <= Rmax; ++j) {
            unsigned cur_pos = i * 80 + j;
            vga_addr[2 * cur_pos] = 0x20;
            vga_addr[2 * cur_pos + 1] = bg_color;
        }
    }
    x = 16, y = 0;
    for (int i = 0; i < 40; ++i) {
        ShowChar();

        /* 设置延迟，实现动态弹球 */
        for (int j = 0; j < delay; ++j) {
            for (int k = 0; k < ddelay; ++k) {
                /* Do nothing */;
            }
        }

        switch (rdul) {
        case 1:
            DownRight();
            break;
        case 2:
            UpRight();
            break;
        case 3:
            UpLeft();
            break;
        case 4:
            DownLeft();
            break;
        }
    }
    SetReturnPos();
    exit();
}

/* 显示运动中的字符 */
void ShowChar() {
    unsigned cur_pos = x * 80 + y;
    ft_color = (ft_color + 1) % 0x10;

    vga_addr[2 * cur_pos] = ch_disp;        /* 设置显示的字符 */
    vga_addr[2 * cur_pos + 1] = ft_color | bg_color;   /* 设置文字颜色 */

    ch_disp = (ch_disp + 1 > 'Z' ? 'A' : ch_disp + 1);
}

void DownRight() {
    x++, y++;
    if (x > Dmax) {
        /* 右下 -> 右上 */
        x = Dmax - 1;
        rdul = Up_Rt;
    }
    else if (y > Rmax) {
        /* 右下 -> 左下 */
        y = Rmax - 1;
        rdul = Dn_Lt;
    }
}

void UpRight() {
    x--, y++;
    if (y > Rmax) {
        /* 右上 -> 左上 */
        y = Rmax - 1;
        rdul = Up_Lt;
    }
    else if (x < Umax) {
        /* 右上 -> 右下 */
        x = Umax + 1;
        rdul = Dn_Rt;
    }
}

void UpLeft() {
    x--, y--;
    if (x < Umax) {
        /* 左上 -> 左下 */
        x = Umax + 1;
        rdul = Dn_Lt;
    }
    else if (y < Lmax) {
        /* 左上 -> 右上 */
        y = Lmax + 1;
        rdul = Up_Rt;
    }
}

void DownLeft() {
    x++, y--;
    if (y < Lmax) {
        /* 左下 -> 右下 */
        y = Lmax + 1;
        rdul = Dn_Rt;
    }
    else if (x > Dmax) {
        /* 左下 -> 左上 */
        x = Dmax - 1;
        rdul = Up_Lt;
    }
}

/* 设置回归光标在最后一行，便于显示*/
void SetReturnPos() {
    asm("int $0x80\n\t"::"a"(6):);
}