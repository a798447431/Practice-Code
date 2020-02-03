/*************************************************************************
	> File Name: forktest.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月26日 星期日 17时03分50秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int x = 0;
    for(int i = 0; i <= 8; ) {
        pid = fork();
        x = i++;
        if (pid == 0)break; 
    }
    if (pid != 0) x++;
    printf("%d\n", x);
    sleep(100000);
    return 0;
}
