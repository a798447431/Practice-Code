/*************************************************************************
	> File Name: testsleep2.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月10日 星期三 21时58分46秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int MyHandler(int sig) {
    (void)sig;
}

int MySleep(int sec) {
    struct sigaction new_action, old_action;
    new_action.sa_handler = MyHandler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction(SIGALRM, &new_action, &old_action);
    
    alarm(sec);
    pause();
    sigaction(SIGALRM, &old_action, NULL);
    return alarm(0);
}

int main() {
    int sec = 0;
    scanf("%d", &sec);
    MySleep(sec);
    return 0;
}
