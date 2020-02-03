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

#define HIGH 33
#define WIGHT 101
#define ROW 10

struct connect_sock {
    int sockfd1;
    int sockfd2;
};

typedef struct ballinfo {
    int people_x;
    int people_y;
    int x;
    int y;
} ballinfo;

ballinfo target;

int move_x = 0;
int move_y = 0;

void *recv_input(void *tmp) {
    struct connect_sock *common = (struct connect_sock *)tmp;
    int sockfd = common->sockfd1;
    while (1) {
        int input = -1;
        if ((input = recv(sockfd, &input, sizeof(input), 0)) < 0) {
            perror("recv() error");
        }
        if (input == 1) {
            move_y = -1;
        } else if (input == 2) {
            move_y = 1;
        }
    }
}

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

void *makeinfo(void *tmp) {;
    struct connect_sock *common = (struct connect_sock *)tmp; 
    int sockfd = common->sockfd1;
    int team = 0;
    if (recv(sockfd, &team, sizeof(team), 0) < 0) {
        perror("recv");
    }
    int player_sock = common->sockfd2;
    int player_sockfd;
    if ((player_sockfd = accept(player_sock, NULL, NULL)) < 0) {
        perror("accept");
        exit(1);
    }
    pthread_t player_input;
    struct connect_sock con;
    con.sockfd1 = player_sockfd;
    pthread_create(&player_input, NULL, recv_input, (void *)&con);
    char ball[10] = "o";
    char blank[2] = " ";
    char player[2] = "F";
    int begin_h = HIGH / 2;
    int begin_w = WIGHT / 2;
    int random_x;
    int random_y;
    random_x = rand() % 32 + 1;
    if (team == 1) {
        random_y = rand() % 54 + 1;                           
    } else if (team == 2) {
        random_y = rand() % 54 + 55;
    }
    initscr();
    clear();
    init_area();
    
    while (1) {
        move(begin_h, begin_w);
        addstr(ball);
        move(LINES - 1, COLS - 1);
        refresh();
        move(random_x, random_y);
        addstr(player);
        refresh();
        target.x = begin_h;
        target.y = begin_w;
        target.people_x = random_x;
        target.people_y = random_y;
        if (send(player_sockfd, &target, sizeof(target), 0) < 0) {
            perror("send");
            break;
        }
        sleep(1);
        move(begin_h, begin_w);
        addstr(blank);

        if(random_x + move_y >= 1 && random_x + move_y <=  HIGH - 1)
            random_x += move_y;
        if(random_y + move_x >= 1 && random_y + move_x <= WIGHT - 1)
            random_y += move_x;
        if(move_y != 0) {
            move_y = 0;
        }
        if(move_x != 0) {
            move_x = 0;
        }
    }
    endwin();
}

int main(int argc, char *argv[]) {
    int server_listen, server_fd, people_listen;
    if (argc != 2) {
        printf("Usage:./server_port!\n");
        exit(1);
    }
    
    int port = atoi(argv[1]);

    if ((server_listen = socket_create(port)) < 0) {
        printf("socket_create: %s\n", strerror(errno));
        exit(1);
    }
    
    if ((people_listen = socket_create(4396)) < 0) {
        printf("socket_create: %s\n", strerror(errno));
        exit(1);
    }

    while (1) {
        pthread_t f;
        if ((server_fd = accept(server_listen, NULL, NULL)) < 0 ) {
            printf("accept: %s\n", strerror(errno));
            exit(1);
        }
        struct connect_sock common_sock;
        common_sock.sockfd1 = server_fd;
        common_sock.sockfd2 = people_listen;
        pthread_create(&f, NULL, makeinfo, (void *)&common_sock);
        
        pthread_join(f, NULL);

    }
    return 0;
}
