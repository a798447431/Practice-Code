/*************************************************************************
	> File Name: itimer.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月11日 星期四 19时17分22秒
 ************************************************************************/

#include <stdio.h> 
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

struct itimerval timer_set;

void sig_handler() {
    printf("hello world!\n");
    timer_set.it_value.tv_sec = 0;
    timer_set.it_value.tv_usec += 20000;
    setitimer(ITIMER_REAL, &timer_set, NULL);
}

int main() {
    signal(SIGALRM, sig_handler);

    timer_set.it_value.tv_sec = 2;
    timer_set.it_value.tv_usec = 0;

    timer_set.it_interval.tv_sec = 0;
    timer_set.it_interval.tv_usec = 200000;
    
    if (setitimer(ITIMER_REAL, &timer_set, NULL) < 0) {
        printf("start timer failed\n");
        return 0;
    }
    
    while (1);
    
    return 0;
}
