/*************************************************************************
	> File Name: exec.c
	> Author: szt
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月28日 星期日 14时33分12秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char tmp[20] = {0};
    int status;
    if (argc != 2) {
        printf("error argument!\n");
        exit(1);
    }
    strcpy(tmp, argv[1]);
    pid_t pid, cpid;
    pid = fork();
    if (pid == 0) {
        if (execl("/usr/bin/vim", "vim", tmp, NULL) < 0) {
            perror("execl");
            exit(1);
        }
    } else {
        waitpid(&pid, &status, 0); 
        cpid = fork();
        if (cpid == 0) {
            wait(&pid);
            if (execl("/usr/bin/gcc", "gcc", tmp, NULL) < 0) {
                perror("execlp");
                exit(1);
            }
        } 
        wait(&cpid);
        system("./a.out");
        printf("execl done\n"); 

    }
    return 0;
}
