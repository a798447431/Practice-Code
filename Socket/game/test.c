/*************************************************************************
	> File Name: test.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月22日 星期六 14时23分06秒
 ************************************************************************/

#include <stdio.h>
#include <curses.h>
#include <unistd.h>

int main() {
    initscr();
    clear();
    move(10, 20);
    addstr("Hello Haizei!\n");
    refresh();
    sleep(1);
    move(10, 20);
    addstr("                ");
    move(11, 21);
    refresh();
    addstr("Hello Haizei!\n");
    getch();
    //pause();
    endwin();
    return 0;
}
