/*************************************************************************
	> File Name: server.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月22日 星期六 15时08分55秒
 ************************************************************************/

#include <stdio.h>
#include <curses.h>
#include <pthread.h>
#include "common.h"

#define LEFTEDGE 10
#define RIGHTEDGE 30
#define ROW 10

typedef struct ballinfo {
    int scr_length;
    int scr_high;
    int x;
    int y;
} ballinfo;

ballinfo target;

void *makeinfo(int *tmp) {
    int *sockfd = (int *)tmp;
    
    char ball[10] = "o";
    char blank[2] = " ";
 
    int dir = 1;
    int pos = LEFTEDGE;

    initscr();
    clear();

    while (1) {
        move(ROW, pos);
        addstr(ball);
        move(LINES - 1, COLS - 1);
        refresh();
        target.x = ROW;
        target.y = pos;
        target.scr_length = RIGHTEDGE - LEFTEDGE;
        target.scr_high = ROW;
        if (send(*sockfd, &target, sizeof(target), 0) < 0) {
            perror("send");
            break;
        }
        sleep(1);
        move(ROW, pos);
        addstr(blank);
        pos += dir;
        if (pos >= RIGHTEDGE) dir = -1;
        if (pos <= LEFTEDGE) dir = 1;
    }
    close(*sockfd);
}


int main(int argc, char *argv[]) {
    int server_listen, server_fd;
    if (argc != 2) {
        printf("Usage:./server_port!\n");
        exit(1);
    }
    int port = atoi(argv[1]);

    if ((server_listen = socket_create(port)) < 0) {
        printf("socket_create: %s\n", strerror(errno));
        exit(1);
    }

    while (1) {
        pthread_t f;
        if ((server_fd = accept(server_listen, NULL, NULL)) < 0 ){
            printf("accept: %s\n", strerror(errno));
            exit(1);
        }
        pthread_create(&f, NULL, makeinfo, (void *)&server_fd);
        pthread_join(f, NULL);
    }
    return 0;
}
