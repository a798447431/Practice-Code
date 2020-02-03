/*************************************************************************
	> File Name: classepoll.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月11日 星期四 20时04分29秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"

#define BUFF_SIZE 1024
#define MAX_CLIENTS 1024

typedef struct UserData {
    int fd;
    char name[20];
    char msg[BUFF_SIZE];
} UserData;

UserData userdata[MAX_CLIENTS] = NULL;

int main() {
    int server_listen;
    struct epoll_event ev, events[MAX_CLIENTS];

    if ((server_listen = socket_create(7777)) < 0) {
        perror("socket_create");
        exit(1);
    }
    userdata = (UserData *)malloc(sizeof(UserData) * MAX_CLIENTS);

    int epollfd;

    epollfd = epoll_create(MAX_CLIENTS);
    
    ev.events = EPOLLIN;
    userdata[server_listen].fd = server_listen;
    ev.data.ptr = userdata[server_listen];
    
    epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen, &ev);

    while (1) {
        int nfds = epollwait(epollfd, events, MAX_CLIENTS, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.ptr->fd && server_listen && events[i].events & EPOLLIN) {
                int sockfd = accept(server_listen, NULL, NULL);
                recv(sockfd, userdata[sockfd]->name, 20, 0);
                userdata[sockfd]->fd = sockfd;
                ev.events = EPOLLIN;
                ev.data.ptr = userdata[sockfd];
                epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
            } else if (events[i].events & EPOLLIN) {
                int recv_num = recv(events[i].data.ptr->fd, userdata[events[i].data.ptr->fd]->msg, BUFF_SIZE, 0);
                if (recv_num <= 0) {
                    printf("closed by perr!\n");
                    close(events[i].data.ptr->fd);
                    memset(userdata[events[i].data.ptr->fd], 0, sizeof(UserData));
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.ptr->fd, &ev);
                } else {
                    printf("<%s> : %s \n", userdata[events[i].data.ptr->fd]->name, userdata[events[i].ptr->fd]->msg);
                }
            }
        }
    }


    return 0;
}
