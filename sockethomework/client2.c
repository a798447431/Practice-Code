/*************************************************************************
	> File Name: n_client.c
	> Author: victoria 
	> Mail: 1105847344@qq.com
	> Created Time: 2019年06月28日 星期五 16时24分51秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"

#define MAX_N 200
#define RIGHTEDGE 110
#define HEIGHT 33


struct Message {
    int socket;
    int team;
    int p_num;
};

struct Point{
    int ball_x;
    int ball_y;
    int p_num;
    int num;
    int lx[MAX_N + 5];
    int ly[MAX_N + 5];
    int team[MAX_N + 5];
};

struct Comm{
    int op;
    int p_num;
};

int P_num;
int flag = 1;
int my_team;

void sigFunc() {
    flag = 0;
}
void *to_recv(void *arg) {
    int *socket = (int *)arg;
    char player[2] = "F";
    char p_str[2] = "P";
    char ball[2] = "o";
    char row_str[2] = "-";
    char mid_line[2] = ".";
    char col_str[2] = "|";
    char men_str[2] = "#";
    initscr();
    clear();
    signal(SIGINT, sigFunc);
    struct Point point ;
    int ball_x, ball_y;
    while (flag) {
        memset(&point, 0, sizeof(point));
        if (recv(*socket, &point, sizeof(point), 0) < 0) {
            perror("error recv : *socket");
            exit(1);
        }
        if (point.num == 0) continue;
        clear();
        P_num = point.p_num;
        for (int i = 0; i <= RIGHTEDGE; i++) {
            move(0, i);
            addstr(row_str);
            move((HEIGHT / 2)+1, i);
            addstr(mid_line);
            move(HEIGHT, i);
            addstr(row_str);
        }
        for (int i = 1; i < HEIGHT; i++) {
            move(i, 0);
            addstr(col_str);
            move(i, RIGHTEDGE/2);
            addstr(col_str);
            move(i, RIGHTEDGE);
            addstr(col_str);
        }
        for (int i = (HEIGHT / 2)-1; i <= (HEIGHT / 2)+3; i++) {
            move(i, 0);
            addstr(men_str);
            if(i == (HEIGHT / 2) - 1 || i == (HEIGHT / 2) + 3) {
                move(i, 1);
                addstr(men_str);
                move(i, 2);
                addstr(men_str);
                move(i, RIGHTEDGE - 1);
                addstr(men_str);
                move(i, RIGHTEDGE - 2);
                addstr(men_str);
            }
            move(i, RIGHTEDGE);
            addstr(men_str);
        }
        ball_x = point.ball_x;
        ball_y = point.ball_y;
        move(ball_x, ball_y);
        addstr(ball);
        for (int i = 0; i < point.num; i++) {
            if (point.lx[i] == -9999 && point.ly[i] == -9999) {
                continue;
            }
            move(point.lx[i], point.ly[i]);
            if (point.p_num == i) {
                addstr(player);
            } else {
                if (point.team[i] == my_team){
                    addstr(p_str);
                } else {
                    addstr(p_str);
                }
            }
        }
        refresh();
    }
    endwin();
}
void to_send(int socket) {
    signal(SIGINT, sigFunc);
    while (flag) {
        char c;
        scanf("%c", &c);
        if (flag == 0) break;
        struct Comm comm;
        memset(&comm, 0, sizeof(comm));
        comm.p_num = P_num;
        if (c == 'w') comm.op = 1;
        else if (c == 's') comm.op = 2;
        else if (c == 'a') comm.op = 3;
        else if (c == 'd') comm.op = 4;
        else if (c == ' ') comm.op = 5;
        if (comm.op >= 1 && comm.op <= 5) {
            if(send(socket, &comm, sizeof(comm),0) < 0) {
                perror("error send : socket");
                break;
            }
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("参数错误\n");
        exit(1);
    }
    int client_socket;
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("error socket : client_socket");
        exit(1);
    }
    int port = atoi(argv[2]);
    char ip[20];
    strcpy(ip, argv[1]);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    if (connect(client_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("error connect : client_socket");
        exit(1);
    }
    my_team = atoi(argv[3]);
    struct Message mes;
    mes.team = my_team;
    if(send(client_socket, &mes, sizeof(mes), 0) < 0) {
        perror("error send : client_socket");
        exit(1);
    }   
    pthread_t pth_recv;
    pthread_create(&pth_recv, NULL, to_recv, &client_socket);
    to_send(client_socket);
    return 0;
}



