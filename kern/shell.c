#include "libc.h"
#define USER_1 42
#define USER_2 52
#define USER_3 62
#define USER_4 72
#define NUM_OF_PROGRAM 4

void Welcoming() {    
    char *program_name[] = {"1.com", "2.com", "3.com", "4.com"};
    int proc_sec_no[] = {USER_1, USER_2, USER_3, USER_4};
    char name[20];
    char str[32];
    int student_id;
    puts("Hello this is ouyhlan's shell program! The user programs which you can use are: ");
    
    for (int i = 0; i < NUM_OF_PROGRAM; ++i) {
        puts(program_name[i]);
    }

    printf("Name:");
    scanf("%s", name);
    printf("Student ID:");
    scanf("%d", &student_id);
    
    printf("My name is %s\nMy student ID is %d\n", name, student_id);
    while (1) {
        puts("");
        printf("%s>", name);
        scanf("%s", str);
        //gets(str);
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
        else if (strncmp(str, "multiexec", 9) == 0 && (strlen(str) == 9)) {
            int proc_num, pid, sec_no_arr[4], num_of_sec_arr[4];
            scanf("%d", &proc_num);
            for (int i = 0; i < proc_num; ++i) {
                scanf("%d", &pid);
                sec_no_arr[i] = proc_sec_no[pid - 1];
                num_of_sec_arr[i] = 10;
            }
            
            multiexec(proc_num, sec_no_arr, num_of_sec_arr);
            continue;
        }
        else if (strncmp(str, "exit", 4) == 0 && (strlen(str) == 4)) {
            puts("\n[System exits normally!]\n");
            break;
        }
        else if (strlen(str) == 0) {
            continue;
        }
        else {
            printf("Wrong input! Please enter again!");
            continue;
        }
        switch (program_id) {
        case 1:
            exec(USER_1, 10);
            break;
        case 2:
            exec(USER_2, 10);
            break;
        case 3:
            exec(USER_3, 10);
            break;
        case 4:
            exec(USER_4, 10);
            break;
        default:
            /* Do nothing */;
        }
    }
}  