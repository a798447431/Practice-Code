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

#define ASK "Do you want another try"

bool get_reponse(char *);
void set_cr_noechomode();
void tty_mode(int); //0 -> get 1 -> set


int main() {
    bool response;
    tty_mode(0);
    set_cr_noechomode();
    response = get_reponse(ASK);
    tty_mode(1);
    printf("\n response = %d\n", response);
    return 0;
}


bool get_reponse(char *ask) {
    printf("%s (y/n)?", ask);
    int input;
    while (1) {
        switch(input = getchar()) {
            case 'Y':
            case 'y': return true;
            case 'N': 
            case 'n':
            case EOF: return false;
        }
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
