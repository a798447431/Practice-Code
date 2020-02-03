/*************************************************************************
	> File Name: flock.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月04日 星期四 20时35分00秒
 ************************************************************************/

#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define PATH "/tmp/lock"

int main() {
    FILE *fp = fopen(PATH, "w");
    pid_t pid;
    flock(fp->_fileno, LOCK_EX);
    printf("已加锁！\n");
    
    pid = fork();
    if (pid == 0) {
        fclose(fp);
        printf("in child!\n");
        fp = fopen(PATH, "r");
        flock(fp->_fileno, LOCK_EX);
        printf("after fopen");
        fflush(stdout);
        exit(0);
    }
    sleep(10);
    fclose(fp);
    wait(NULL);
    return 0;
}
