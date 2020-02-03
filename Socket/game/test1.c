/*************************************************************************
	> File Name: test1.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月22日 星期六 14时37分57秒
 ************************************************************************/

#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#define LEFTDGE 10
#define RIGHTDGE 30
#define ROW 10

int main() {
    char ball[10] = "o";
    char blank[2] = " ";
    
    int dir = 1;
    int pos = LEFTDGE;

    initscr();
    clear();

    while (1) {
        move(ROW, pos);
        addstr(ball);
        move(LINES - 1, COLS - 1);
        refresh();
        sleep(1);
        move(ROW, pos);
        addstr(blank);
        pos += dir;

        if (pos >= RIGHTDGE) dir = -1;
        if (pos <= LEFTDGE) dir = 1;
    }
    return 0;
}
