/*************************************************************************
	> File Name: exec1.c
	> Author: szt
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月28日 星期日 17时32分12秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int pid, pid1, pid2, status;
    char filename[100] = {0};
    char filename1[100] = {0};
    char runstr[150] = {0};

    if (argc != 2) {
        printf("Usage:./test source.code\n");
        exit(1);
    }
    strcpy(filename, argv[1]);

    char *sub;
    char name[20] = {0};
    sub = strrchr(filename, ".");
    strncpy(name, filename, sub - filename);
    char g_flag[5] = {0};
    char g_run[20] = {0};
    if (strcmp(sub, ".c") == 0) {
        strcpy(g_flag, ".gcc");
    } else if (strcmp(sub, ".cpp") == 0) {
        strcpy(g_flag, "g++");
    } else {
        printf("文件格式不支持!\n");
        exit(1);
    }

    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        execlp("/usr/bin/vim", "vim", filename, NULL);
    }
    wait(NULL);
    
    if ((pid1 = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    
    if ((pid1 = fork()) == 0) {
        sprintf(g_run, "/usr/bin/%s", g_flag);
        execlp(g_run, g_flag, "-lm", filename, "-o", name, NULL);
    }
    
    wait(&status);
    
    if (status != 0) {
        printf("编译错误!\n");
        exit(1);
    }
    
    char run_name[50] = {0};
    sprintf(run_name, "./%s", name);
    execl(run_name, NULL);
    return 0;
}
