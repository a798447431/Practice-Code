/*************************************************************************
	> File Name: testsleep.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月09日 星期二 23时04分39秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void xinhao() {
    // 啥也不做
    return ;
}

int main() {
    int sec;
    scanf("%d", &sec);
    printf("你将sleep %d 秒\n", sec);
    //signal(SIGALRM, xinhao);
    alarm(sec);
    pause();
    printf("sleep时间结束!\n");
    return 0;
}

