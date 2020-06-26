#include "libc.h"
int strlen(char *str) {

    int count = 0;
    while (str[count++] != '\0') {}
    return count - 1;
}