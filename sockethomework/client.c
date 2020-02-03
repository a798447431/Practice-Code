/*************************************************************************
	> File Name: client.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月03日 星期三 22时33分34秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"

#define MAX_N 100
#define RIGHTEDGE 110
#define HEIGHT 33

struct Info {
    int sockfd;
    int team;
    int p_num;
};

struct Point {
    int ball_x;
    int ball_y;
    int p_num;
    int num;
    int lx[MAX_N + 5];
    int ly[MAX_N + 5];
    int team[MAX_N + 5];
};

struct Instruct {
    int input;
    int p_num;
};

int P_num;
int flag = 1;
int my_team;

void sigFunc() {
    flag = 0;
}

void *recv_info(void *arg) {
    int *sockfd = (int *)arg;
    char player[2] = "F";
    char p_str[2] = "P";
    char ball[2] = "o";
    char row_str[2] = "-";
    char mid_line[2] = ".";
    char col_str[2] = "|";
    char door[2] = "#";
    initscr();
    clear();
    signal(SIGINT, sigFunc);
    struct Point point;
    int ball_x, ball_y;
    
    while (flag) {
        memset(&point, 0, sizeof(point));
        if (recv(*sockfd, &point, sizeof(point), 0) < 0) {
            perror("recv() error!\n");
            exit(1);
        }
        if (point.num == 0) continue;
        clear();
        P_num = point.p_num;
        
        for (int i = 0; i <= RIGHTEDGE; i++) {
            move(0, i);
            addstr(row_str);
            move((HEIGHT / 2) + 1, i);
            addstr(mid_line);
            move(HEIGHT, i);
            addstr(row_str);
        }
        
        for (int i = 1; i < HEIGHT; i++) {
            move(i, 0);
            addstr(col_str);
            move(i, RIGHTEDGE / 2);
            addstr(col_str);
            move(i, RIGHTEDGE);
            addstr(col_str);
        }

        for (int i = (HEIGHT / 2) - 1; i <= (HEIGHT / 2) + 3; i++) {
            move(i, 0);
            addstr(door);
            if (i == (HEIGHT / 2) - 1 || i == (HEIGHT / 2) + 3) {
                move(i, 1);
                addstr(door);
                move(i, 2);
                addstr(door);
                move(i, RIGHTEDGE - 1);
                addstr(door);
                move(i, RIGHTEDGE - 2);
                addstr(door);
            }
            move(i, RIGHTEDGE);
            addstr(door);
        }
        ball_x = point.ball_x;
        ball_y = point.ball_y;
        move(ball_x, ball_y);
        addstr(ball);

        for (int i = 0; i <= point.num; i++) {
            if (point.lx[i] == -9999 && point.ly[i] == -9999) {
                continue;
            }
            move(point.lx[i], point.ly[i]);
            if (point.p_num == i) {
                addstr(player);
            } else {
                addstr(p_str);
            }
        }
        refresh();
    }
    endwin();
}

void send_input(int socket) {
    signal(SIGINT, sigFunc);
    while (flag) {
        char c;
        scanf ("%c", &c);
        if (flag == 0) break;
        struct Instruct instruct;
        memset(&instruct, 0, sizeof(instruct));
        instruct.p_num = P_num;
        if (c == 'w') instruct.input = 1;
        else if (c == 's') instruct.input = 2;
        else if (c == 'a') instruct.input = 3;
        else if (c == 'd') instruct.input = 4;
        else if (c == 'f') instruct.input = 5;

        deepin svnif (instruct.input >= 1 && instruct.input <= 5) {
            if (send(socket, &instruct, sizeof(instruct), 0) < 0) {
                perror("send() error!\n");
                break;
            }
        }
    }
}

int main(int argc, char *argv[])  {
    if (argc != 4) {
        printf("error paramaeter!\n");
        exit(1);
    }
    int client_sockfd;
    int port = atoi(argv[2]);
    char ip[20];
    strcpy(ip, argv[1]);
    if ((client_sockfd = socket_connect(port, ip)) < 0) {
        perror("socket_connect() error!\n");
        exit(1);
    }
    my_team = atoi(argv[3]);
    struct Info info;
    info.team = my_team;
    if (send(client_sockfd, &info, sizeof(info), 0) < 0) {
        perror("send() error!\n");
        exit(1);
    }
    pthread_t pth_recv;
    pthread_create(&pth_recv, NULL, recv_info, &client_sockfd);
    send_input(client_sockfd);
    return 0;
}
