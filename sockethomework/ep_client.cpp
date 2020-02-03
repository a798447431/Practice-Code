/*************************************************************************
	> File Name: ep_client.cpp
	> Author: victoria 
	> Mail: 1105847344@qq.com
	> Created Time: 2019年07月12日 星期五 15时25分09秒
 ************************************************************************/

#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <curses.h>
#include <pwd.h>
#include "common_n.h"
using namespace std;

#define MAX_N 200
#define RIGHTEDGE 110
#define HEIGHT 33

typedef struct Point{
    int ball_x;
    int ball_y;
    int p_num;
    int max_num;
    int score_1;
    int score_2;
    int lx[MAX_N + 5];
    int ly[MAX_N + 5];
    int team[MAX_N + 5];
}Point;

typedef struct Comm{
    int op;
    int p_num;
    int team;
    char name[20];
}Comm;

Comm comm;
int flag = 1;
int my_num;


void sigFunc(int) {
    flag = 0;
}

void draw_ground() {
    char row_str[2] = "-";
    char mid_line[2] = ".";
    char col_str[2] = "|";
    char men_str[2] = "#";

    for (int i = 0; i <= RIGHTEDGE; i++) {
        move(0, i);
        addstr(row_str);
        move((HEIGHT / 2)+1, i);
        addstr(mid_line);
        move(HEIGHT, i);
        addstr(row_str);
    }
    for (int i = 10; i < 24; i++) {
        move(i, 45);
        addstr(mid_line);
        move(i, 65);
        addstr(mid_line);
    }
    for (int i = 45; i <= 65; i++) {
        move(10, i);
        addstr(mid_line);
        move(23, i);
        addstr(mid_line);
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
}

void draw_player(Point game_msg) {
    char player[2] = "F";
    char p_str[2] = "P";
    
    for (int i = 0; i < game_msg.max_num; i++) {
        move(game_msg.lx[i], game_msg.ly[i]);
        if (game_msg.p_num == i) {
            addstr(player);
        } else {
            if (game_msg. team[i] == comm.team){
                addstr(p_str);
            } else {
                addstr(p_str);
            }
        }
    }
}

void do_send(int client_socket) {
    //signal(SIGINT, sigFunc);
    while (flag) {
        char c;
        cin >> c;
        if (flag == 0) break;
        comm.p_num = my_num;
        if (c == 'w') comm.op = 1;
        else if (c == 's') comm.op = 2;
        else if (c == 'a') comm.op = 3;
        else if (c == 'd') comm.op = 4;
        else if (c == 'j') comm.op = 5;
        if (comm.op >= 1 && comm.op <= 5) {
            send(client_socket, &comm, sizeof(comm), 0);
        } 
    }
}

void *to_recv(void *arg) {
    int *socket = (int*)arg;
    //signal(SIGINT, sigFunc);
    initscr();
    clear();
    struct Point game_msg;
    char ball[2] = "O";
    while (flag) {
        memset(&game_msg, 0, sizeof(game_msg));
        if ( recv(*socket, &game_msg, sizeof(game_msg), 0) < 0) {
            perror("error recv : *socket");
            exit(1);
        } 
        my_num = game_msg.p_num;
        if (game_msg.max_num == 0) continue;
        clear();
        draw_ground();
        move(game_msg.ball_x, game_msg.ball_y);
        addstr(ball);
        draw_player(game_msg);
        refresh();
    }
    endwin();
}

int main(int argc, char **argv) {
    if (argc != 4) {
        cout << "参数错误" << endl;
        exit(1);
    }
    int client_socket;
    char ip[20] ;
    strcpy(ip, argv[1]);
    int port = atoi(argv[2]);
    if ((client_socket = socket_connect(port, ip)) < 0) {
        perror("socket_connect : client_socket");
        exit(1);
    }
    comm.team = atoi(argv[3]);
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    strcpy(comm.name, pwd->pw_name);
    send(client_socket, &comm, sizeof(comm), 0);
    pthread_t pth_recv;
    pthread_create(&pth_recv, NULL, to_recv, &client_socket);
    do_send(client_socket);
    close(client_socket);
    return 0;
}





