#include "libc.h"
/* Compare two string 
** 0 for same, -1 for not the same  
*/
int strncmp(char *str1, char *str2, unsigned int n) {
    for (int i = 0; i < n; ++i) {
        if (str1[i] != str2[i])
            return -1;
    }
    return 0;
}
