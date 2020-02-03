/*************************************************************************
	> File Name: testpanduan1.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月16日 星期二 14时17分17秒
 ************************************************************************/

#include <stdio.h>
#include <stdbool.h> 
#include <termios.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define ASK "Do you want another try"
#define TIMEOUT 5
int get_response(char *);
void set_cr_noecho_mode();
void tty_mode(int); // 0 -> get 1 -> set
void set_noblock();

int termflags;

int main() {
    int response;
    tty_mode(0);
    set_noblock();
    set_cr_noecho_mode();
    response = get_response(ASK);
    tty_mode(1);
    printf("\n response = %d\n", response);
    return 0;
}

int get_response(char *ask) {
    printf("%s (y/n)?", ask);
    fflush(stdout);
    int get_ok_char();
    int input, ret;
    struct timeval tv;
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
    fd_set set;
    FD_ZERO(&set);
    FD_SET(0, &set);
    
    ret = select(1, &set, NULL, NULL, &tv);

    if (ret == 1) {
        if (FD_ISSET(0, &set)) {
            while (1) {
                input = get_ok_char();
                switch(input) {
                    case 'y': return 1;
                    case 'n': return 0;
                    default : goto a;
                }
            }
        }
    }
    return 2;
}

void set_cr_noecho_mode() {
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

void set_noblock() {
    int termflags;
    termflags = fcntl(0, F_GETFL);
    termflags |= O_NONBLOCK;
    fcntl(0, F_SETFL, termflags);
}

int get_ok_char() {
    int c;
    while((c = getchar()) != EOF && (strchr("yYnN",c) == NULL));
    return c;
}
