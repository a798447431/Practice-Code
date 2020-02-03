/*************************************************************************
	> File Name: testpanduan.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月16日 星期二 14时08分20秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <curses.h>

int re_game() {
    initscr();
    clear();
    move(10, 20);
    addstr("是否继续游戏");
    move(12, 22);
    addstr("Yes or No?");
    refresh();
    char c;
    scanf("%c", c);
    if (c == "y") return 0;
    else if (c == "n") return 1;
}



int main() {
    if (re_game() == 0) {
        return re_game();
    }
    else if (re_game() == 1){
        return 0;
    }
    return 0;
}
