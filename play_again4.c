/*************************************************************************
	> File Name: play_again1.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月20日 星期六 09时50分58秒
 ************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#define ASK "Do you want another try"
#define TRY 3
#define SLEEPTIME 5

int get_reponse(char *);
void set_cr_noechomode();
void tty_mode(int); //0 -> get 1 -> set
void set_nonblock_mode();

int main() {
    int response;
    tty_mode(0);
    set_cr_noechomode();
    set_nonblock_mode();
    response = get_reponse(ASK);
    tty_mode(1);
    printf("\n response = %d\n", response);
    return 0;
}


int get_reponse(char *ask) {
    printf("%s (y/n)?", ask);
    int get_ok_char();
    int input;
    int maxtry = TRY;
    while (1) {
        printf("\nTimes left <%d>", maxtry);
        input = tolower(get_ok_char());
        if (input == 'y') return 1;
        if (input == 'n') return 0;
        if (maxtry-- == 0) return 2;
        sleep(SLEEPTIME);
    }
}

void set_cr_noechomode() {
    struct termios ttystate;
    tcgetattr(0, &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_lflag &= ~ECHO;
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int how) {
    static struct termios orignal_mode;
    if (how == 0) {
        tcgetattr(0, &orignal_mode);
    } else {
        tcsetattr(0, TCSANOW, &orignal_mode);
    }
}

void set_nonblock_mode() {
    int termflags;
    termflags = fcntl(0, F_GETFL);
    termflags |= O_NONBLOCK;
    fcntl(0, F_SETFL, termflags);
}

int get_ok_char() {
    int c;
    while ((c = getchar()) != EOF && strchr("yYnN", c) == NULL);
    return c;
}
