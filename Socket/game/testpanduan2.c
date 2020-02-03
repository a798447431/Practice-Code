/*************************************************************************
	> File Name: testpanduan1.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月16日 星期二 14时17分17秒
 ************************************************************************/

#include <stdio.h>
#include <stdbool.h> 
#include <termios.h>
#define ASK "Do you want another try"


bool get_response(char *);
void set_crmode();
void tty_mode(int); // 0 -> get 1 -> set

int main() {
    bool response;
    tty_mode(0);
    set_crmode();
    response = get_response(ASK);
    tty_mode(1);
    printf("\n response = %d\n", response);
    return 0;
}

bool get_response(char *ask) {
    printf("%s (y/n)?", ask);
    while (1) {
        switch(getchar()) {
            case 'Y':
            case 'y': return true; 
            case 'N':
            case 'n': 
            case 'EOF': return false;
        }
    }
}

void set_crmode() {
    struct termios ttystate;
    tcgetattr(0, &ttystate);
    ttystate.c_lflag &= ~(ICANON | ECHO) ;
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int x) {
    static struct termios orignal_mode;
    if (x == 0) {
        tcgetattr(0, &orignal_mode);
    } else if (x == 1) {
        tcsetattr(0, TCSANOW, &orignal_mode);
    }
}
