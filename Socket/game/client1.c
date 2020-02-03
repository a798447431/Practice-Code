/*************************************************************************
	> File Name: client.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月22日 星期六 14时53分32秒
 ************************************************************************/

#include <stdio.h>
#include <curses.h>
#include "common.h"

typedef struct ballinfo {
    int scr_length;
    int scr_high;
    int x;
    int y;
} ballinfo;

ballinfo target;

void recv_game(int sockfd) {
    char ball[10] = "o";
    char blank[2] = " ";

    initscr();
    clear();

    while (1) {
        if(recv(sockfd, &target, sizeof(target), 0) < 0) {
            perror("recv");
        }
        printf("%d, %d", target.x, target.y);
        int row = target.x;
        int lie = target.y;
        int size_l = target.scr_length;
        int size_h = target.scr_high;
        move(row, lie);
        addstr(ball);
        move(LINES - 1, COLS - 1);
        refresh();
        sleep(1);
        move(row, lie);
        addstr(blank);
        refresh();
    }
    close(sockfd);
}

int main(int argc, char *argv[]) {
    int sock_scr; 
    
    if (argc != 3) {
        printf("Usage:./client_ip, port!\n");
        exit(1);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    
    if ((sock_scr = socket_connect(port, ip)) < 0) {
        printf("Error in socket_connect: %s\n", strerror(errno));
        exit(1);
    }
    recv_game(sock_scr);
    //close(sock_scr);
    return 0;
}
