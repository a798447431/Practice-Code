#include <stdio.h>
#include "common.h"

#define MAX_N 200
#define RIGHTEDGE 110
#define HEIGHT 33

int P_socket[MAX_N + 5];
int loc_x[MAX_N + 5];
int loc_y[MAX_N + 5];
int P_team[MAX_N + 5];
int Index;
int ball_x;
int ball_y;
int speed;
int yd_row ;
int yd_col ;

struct Message {
    int socket;
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

struct Comm {
    int op;
    int p_num;
};

bool out_Range(int x, int y) {
    if (x <= 0 || x >=HEIGHT || y <= 0 || y >= RIGHTEDGE) {
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

void *to_send () {
    char p_str[2] = "P";
    char ball[2] = "o";
    char mid_line[2] = ".";
    char men_str[2] = "#";
    char row_str[2] = "-";
    char col_str[2] = "|";
    ball_x = HEIGHT / 2 + 1;  
    ball_y = RIGHTEDGE / 2;
    initscr();
    clear();
    char ball_info[100];
    char score01_info[100];
    char score02_info[100];
    int score_team1 = 0;
    int score_team2 = 0;
    while (1) {
        for (int i = 0; i <= RIGHTEDGE; i++) {
            move(0, i);
            addstr(row_str);
            move(HEIGHT/2+1, i);
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
        if(speed > 0) {
            ball_x += yd_row;
            ball_y += yd_col;
            move(ball_x, ball_y);
            addstr(ball);
            speed -= 1;
        } else {
            move(ball_x, ball_y);
            addstr(ball);
            
            yd_row = 0;
            yd_col = 0;
        }
        int k = is_win(ball_x, ball_y);
        if (k > 0) {
            ball_x = HEIGHT / 2 + 1;  
            ball_y = RIGHTEDGE / 2;
            speed = 0;
        }
        if (out_Range(ball_x, ball_y)) {
            ball_x = HEIGHT / 2 + 1;  
            ball_y = RIGHTEDGE / 2;
            speed = 0;
        }
        struct Point point ;
        memset(&point, 0, sizeof(point));
        point.ball_x = ball_x;
        point.ball_y = ball_y;
        for(int i = 0; i < Index; i++) {
            if (P_socket[i] < 0) {
                point.lx[i] = -9999;
                point.ly[i] = -9999;
                continue;
            }
            point.team[i] = P_team[i];
            move(loc_x[i], loc_y[i]);
            if (P_team[i] == 1) {
                addstr(p_str);
            } else if (P_team[i] == 2) {
                addstr(p_str);
            }
            point.lx[i] = loc_x[i];
            point.ly[i] = loc_y[i];
        }
        refresh();
        point.num = Index;
        for (int i = 0; i < Index; i++) {
            if (P_socket[i] < 0) {
                continue;
            }
            point.p_num = i;
            send(P_socket[i], &point, sizeof(point), 0);
        }
        usleep(100000);
        clear();
    };
    endwin();
}

int dest[8][2] = {-1, 0, 1, 0, 0, -1, 0, 1, -1, 1, 1, 1, 1, -1, -1, -1};
bool has_ball(int row, int col) {
    if (fabs(row - ball_x) <= 1 && fabs(col - ball_y) <= 1) {
        for (int i = 0;i < 8; i++) {
            int nrow = row + dest[i][0];
            int ncol = col + dest[i][1];
            if (nrow == ball_x && ncol == ball_y) {
                yd_row = dest[i][0] * 2;
                yd_col = dest[i][1] * 2;
                break;
            }
        }
        return true;       
    } else {
        return false;
    }
}

void *to_recv(void *arg){
    struct Message *mes = (struct Message*)arg;
    int socket = mes->socket;
    while(1) {
        struct Comm comm;
        memset(&comm, 0, sizeof(comm));
        if (recv(socket, &comm, sizeof(comm), 0) <= 0) {
            //close(socket);
            loc_x[comm.p_num] = -9999;
            loc_y[comm.p_num] = -9999;
            P_socket[comm.p_num] = -1;
            break;
        }
        switch(comm.op) {
            case 1:
                loc_x[comm.p_num] -= 1;
                break;
            case 2:
                loc_x[comm.p_num] += 1;
                break;
            case 3:
                loc_y[comm.p_num] -= 1;
                break;
            case 4:
                loc_y[comm.p_num] += 1;
                break;
            case 5:
                if (has_ball(loc_x[comm.p_num], loc_y[comm.p_num])) {
                    speed = 6; 
                }
                break;
            default:
                break;
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("参数错误\n");
        exit(1);
    }

    int port = atoi(argv[1]);
    int listen_socket;
    if((listen_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("error socket : listen_socket");
        exit(1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listen_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("error bind : listen_socket");
        exit(1);
    }
    listen(listen_socket, 20);
    pthread_t pth_send;
    pthread_create(&pth_send, NULL, to_send, NULL);
    while (1) {
        int ball_fd;
        if ((ball_fd = accept(listen_socket, NULL, NULL)) < 0) {
            perror("error accept : ball_fd");
            exit(1);
        }
        struct Message mes;
        memset(&mes, 0, sizeof(mes));
        if (recv(ball_fd, &mes, sizeof(mes), 0) < 0) {
            perror("error recv : ball_fd");
            exit(1);
        }
        srand((unsigned)time(0) * 10);
        loc_x[Index] = rand() % 32 + 1;
        P_team[Index] = mes.team;
        if (mes.team == 1) {
            loc_y[Index] = rand() % 54 + 1;
        } else if (mes.team == 2) {
            loc_y[Index] = rand() % 54 + 56;
        }
        P_socket[Index] = ball_fd;
        mes.p_num = Index;

        Index += 1;

        mes.socket = ball_fd;
        pthread_t pth_recv;
        pthread_create(&pth_recv, NULL, to_recv, &mes);
    }
    close(listen_socket);
    return 0;
}
