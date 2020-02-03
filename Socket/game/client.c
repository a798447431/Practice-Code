/*************************************************************************
	> File Name: client.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月22日 星期六 14时53分32秒
 ************************************************************************/

#include <stdio.h>
#include <curses.h>
#include <pthread.h>
#include "common.h"

#define HIGH 33
#define WIGHT 101

typedef struct ballinfo {
    int people_x;
    int people_y;
    int x;
    int y;
} ballinfo;

struct 

ballinfo target;

void init_area() {
    char gm_wight[2] = "-";
    char gm_high[2] = "|";
    for (int i = 0; i < WIGHT; i++) {
        move(0, i);
        addstr(gm_wight);
        move(HIGH, i);
        addstr(gm_wight);
        move(HIGH / 2, i);
        addstr(gm_wight);
    }
    
    for (int j = 0; j < HIGH; j++) {
        move(j, 0);
        addstr(gm_high);
        move(j, WIGHT);
        addstr(gm_high);
        move(j, WIGHT / 2);
        addstr(gm_high);
    }

}

void *recv_game(void *tmp) {
    int *sockfd = (int *)tmp;
    char ball[10] = "o";
    char blank[2] = " ";
    char player[2] = "F";
    initscr();
    clear();
    init_area(); 
    while (1) {
        if(recv(*sockfd, &target, sizeof(target), 0) < 0) {
            perror("recv");
            break;
        }
        int row = target.x;
        int lie = target.y;
        int people_x = target.people_x;
        int people_y = target.people_y;
        move(row, lie);
        addstr(ball);
        move(LINES - 1, COLS - 1);
        refresh();
        move(people_x, people_y);
        addstr(player);
        move(LINES - 1, COLS - 1);
        move(people_x, people_y);
        addstr(blank);
    }
    endwin();
    close(*sockfd);
}

void send_input(int sockfd) {
    while(1) {
        char c;
        printf("%c", &c);
        int code_num = -1;
        if (c == '0') break;
        if (c == 'a') code_num = 1;
        else if (c == 'd') code_num = 2;
        if (code_num != -1)
        if (send(sockfd, &code_num, sizeof(code_num), 0) < 0) {
            perror("send");
            break;
        }
    } 
}


int main(int argc, char *argv[]) {
    int sock_scr, sock_input; 
    
    if (argc != 4) {
        printf("Usage:./client_ip, port!\n");
        exit(1);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    
    if ((sock_scr = socket_connect(port, ip)) < 0) {
        printf("Error in socket_connect: %s\n", strerror(errno));
        exit(1);
    }
    
    int term = atoi(argv[3]);
    
    if (send(sock_scr, &term, sizeof(term), 0) < 0) {
        perror("send");
    }
    
    int port2 = 4396;
    
    if ((sock_input = socket_connect(port2, ip)) < 0) {
        printf("Error in socket_connect: %s\n", strerror(errno));
        exit(1);
    }

    pthread_t pth_recv;
    pthread_create(&pth_recv, NULL, recv_game, (void *)&sock_input);
    send_input(sock_input);
    close(sock_scr);
    return 0;
}
