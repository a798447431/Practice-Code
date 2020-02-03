/*************************************************************************
	> File Name: server.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月03日 星期三 19时44分24秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"

#define MAX_N 100
#define RIGHTEDGE 110
#define HEIGHT 33

int all_socket[MAX_N + 5];
int random_x[MAX_N + 5];
int random_y[MAX_N + 5];
int all_team[MAX_N + 5];
int ans;
int ball_x;
int ball_y;
int space;
int move_row;
int move_col;

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
bool out_Range(int x, int y) {
    if (x <= 0 || x >= HEIGHT || y <= 0 || y >= RIGHTEDGE) {
        return true;
    }
    return false;
}

int is_win(int x, int y) {
    if (x < (HEIGHT / 2) - 1 && x > (HEIGHT / 2) + 3) {
        return 0;
    }
    if (y <= 0) return 1;
    else if (y >= RIGHTEDGE) return 2;
    else return 0;
}

void *send_map() {
    char penson[2] = "P";
    char ball[2] = "o";
    char mid_line[2] = ".";
    char door[2] = "#";
    char row_str[2] = "-";
    char col_str[2] = "|";
    ball_x = HEIGHT / 2 + 1;
    ball_y = RIGHTEDGE / 2;
    initscr();
    clear();
    while (1) {
        for (int i = 0; i <= RIGHTEDGE; i++) {
            move(0, i);
            addstr(row_str);
            move(HEIGHT / 2 + 1, i);
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
                if (i == (HEIGHT /2 ) - 1 || i == (HEIGHT /2 ) + 3) {
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
        
        if(space > 0) {
            ball_x += move_row;
            ball_y += move_col;
            move(ball_x, ball_y);
            addstr(ball);
            space -= 1;
        } else {
            move(ball_x, ball_y);
            addstr(ball);

            move_row = 0;
            move_col = 0;
        }

        int k = is_win(ball_x, ball_y);
        if (k > 0) {
            ball_x = HEIGHT / 2 + 1;
            ball_y = RIGHTEDGE / 2;
            space = 0;
        }
        if (out_Range(ball_x, ball_y)) {
            ball_x = HEIGHT / 2 + 1;
            ball_y = RIGHTEDGE / 2;
            space = 0;
        }
        struct Point point;
        memset(&point, 0, sizeof(point));
        point.ball_x = ball_x;
        point.ball_y = ball_y;
        for (int i = 0; i < ans; i++) {
            if (all_socket[i] < 0) {
                point.lx[i] = -9999;
                point.ly[i] = -9999;
                continue;
            }
            point.team[i] = all_team[i];
            move(random_x[i], random_y[i]);
            addstr(penson);
            point.lx[i] = random_x[i];
            point.ly[i] = random_y[i];
        }
        refresh();
        point.num = ans;
        for (int i = 0; i < ans; i++) {
            if (all_socket[i] < 0) {
                continue;
            }
            point.p_num = i;
            send(all_socket[i], &point, sizeof(point), 0);
        }
        usleep(100000);
        clear;
    };
    endwin();
}

int dest[8][2] = {-1, 0, 1, 0, 0, -1, 0, 1, -1, 1, 1, 1, 1, -1, -1, -1};

bool has_ball(int row, int col) {
    if (fabs(row - ball_x) <= 1 && fabs(col -  ball_y) <= 1) {
        for (int i = 0; i < 8; i++) {
            int nrow = row + dest[i][0];
            int ncol = col + dest[i][1];
            if (nrow == ball_x && ncol == ball_y) {
                move_row = dest[i][0] * 2;
                move_col = dest[i][1] * 2;
                break;
            }
        }
        return true;
    } else {
        return false;
    }
}

void *recv_info(void *arg) {
    struct Info *info = (struct Info*)arg;
    int sockfd = info->sockfd;
    while(1) {
        struct Instruct instruct;
        memset(&instruct, 0, sizeof(instruct));
        if (recv(sockfd, &instruct, sizeof(instruct), 0) <= 0) {
            close(sockfd);
            random_x[instruct.p_num] = -9999;
            random_y[instruct.p_num] = -9999;
            all_socket[instruct.p_num] = -1;
            break;
        }

        switch(instruct.input) {
            case 1:
                random_x[instruct.p_num] -= 1;
                break;
            case 2:
                random_x[instruct.p_num] += 1;
                break;
            case 3:
                random_y[instruct.p_num] -= 1;
                break;
            case 4:
                random_y[instruct.p_num] += 1;
                break;
            case 5:
            if (has_ball(random_x[instruct.p_num], random_y[random_y[instruct.p_num]])) {
                space = 6;       
            }
                break;
            default:
                break;
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("error paramaeter!\n");
        exit(1);
    }
    int port = atoi(argv[1]);
    int server_listen;
    if ((server_listen = socket_create(port)) < 0) {
        printf("socket_create error!\n");
        exit(1);
    }

    pthread_t pth_sendinfo;
    pthread_create(&pth_sendinfo, NULL, send_map, NULL);

    while (1) {
        int ball_fd;
        if ((ball_fd = accept(server_listen, NULL, NULL)) < 0) {
            printf("accept() error!\n");
            exit(1);
        }
        struct Info info;
        memset(&info, 0, sizeof(info));
        if (recv(ball_fd, &info, sizeof(info), 0) < 0) {
            perror("recv() error!\n");
            exit(1);
        }
        srand((unsigned)time(0) * 10);
        random_x[ans] = rand() % 32 + 1;
        all_team[ans] = info.team;
        if (info.team == 1) {
            random_y[ans] = rand() % 54 + 1;
        } else if (info.team == 2) {
            random_y[ans] = rand() % 54 + 56;
        }
        all_socket[ans] = ball_fd;
        info.p_num = ans;
        ans += 1;
        info.sockfd = ball_fd;
        pthread_t pth_recv;
        pthread_create(&pth_recv, NULL, recv_info, &info);
    }
    close(server_listen);
    return 0;
}
