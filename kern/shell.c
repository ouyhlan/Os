#include "libc.h"
#define USER_1 32
#define USER_2 36
#define USER_3 40
#define USER_4 44
#define NUM_OF_PROGRAM 4

void Welcoming() {    
    char *program_name[] = {"1.com", "2.com", "3.com", "4.com"};
    char name[20];
    char str[32];
    int student_id;
    puts("Hello this is ouyhlan's monitor program! The following programs which you can use: ");
    
    for (int i = 0; i < NUM_OF_PROGRAM; ++i) {
        puts(program_name[i]);
    }

    printf("Name:");
    scanf("%s", name);
    printf("Student ID:");
    scanf("%d", &student_id);
    
    printf("My name is %s\nMy student ID is %d\n", name, student_id);
    while (1) {
        printf("%s> ", name);
        gets(str);
        int program_id = 0;
        if (strncmp(str, "1.com", 5) == 0 && (strlen(str) == 5)) {
            program_id = 1;
        }
        else if (strncmp(str, "2.com", 5) == 0 && (strlen(str) == 5)) {
            program_id = 2;
        }
        else if (strncmp(str, "3.com", 5) == 0 && (strlen(str) == 5)) {
            program_id = 3;
        }
        else if (strncmp(str, "4.com", 5) == 0 && (strlen(str) == 5)) {
            program_id = 4;
        }
        else if (strncmp(str, "int 22h", 7) == 0 && (strlen(str) == 7)) {
            asm("int $0x22\n\t");
        }
        else if (strncmp(str, "exit", 4) == 0 && (strlen(str) == 4)) {
            puts("\n[System exits normally!]\n");
            break;
        }
        else if (strlen(str) == 0) {
            continue;
        }
        else {
            puts("Wrong input! Please enter again!\n");
            continue;
        }
        switch (program_id) {
        case 1:
            exec(USER_1, 4);
            break;
        case 2:
            exec(USER_2, 4);
            break;
        case 3:
            exec(USER_3, 4);
            break;
        case 4:
            exec(USER_4, 4);
            break;
        default:
            /* Do nothing */;
        }
        puts("");
    }
}  