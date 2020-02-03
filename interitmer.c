/*************************************************************************
	> File Name: interitmer.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月16日 星期二 11时03分59秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

struct itimerval timer;

void print() {
    printf("Hello HaiZei!\n");
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec += 20000;
    setitimer(ITIMER_REAL, &timer, NULL);
}

int main() {
    signal(14, print);
    struct itimerval timer;
    timer.it_value.tv_sec = 2;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    
    setitimer(ITIMER_REAL, &timer, NULL);

    while (1);
    return 0;
}
