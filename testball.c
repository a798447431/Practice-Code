/*************************************************************************
	> File Name: testball.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月16日 星期二 09时49分36秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define LEFTDGE 10
#define RIGHTDGE 110
#define ROW 10

struct itimerval timer_set;

int dir = 30; 
int pos = LEFTDGE;

void sig_handler() {
    if (dir <= 0) return ;
    dir -= 1;
    pos += (dir / 10);
}

int main() {
    signal(SIGALRM, sig_handler);
    timer_set.it_value.tv_sec = 0;
    timer_set.it_value.tv_usec = 80000;

    timer_set.it_interval.tv_sec = 0;
    timer_set.it_interval.tv_usec = 80000; 

    char ball[2] = "o";
    
    if (setitimer(ITIMER_REAL, &timer_set, NULL) < 0) {
        printf("start timer failed\n");
        return 0;
    }

    initscr();
    clear();
   
    while (1) {
        move(ROW, pos);
        addstr(ball);
        refresh();
        clear();    
    }
    return 0;
}
