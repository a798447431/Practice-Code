/*************************************************************************
	> File Name: servertest.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月10日 星期三 18时23分52秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"
#define MAX_N 100
#define PORT 8888 
#define MAX_BUFF 1024

char all_buff[MAX_N + 5][MAX_BUFF + 5] = {0};

int main() {
    int server_listen;
    server_listen = socket_create(PORT);
    if (server_listen < 0) {
        printf("socket_create() error!\n");
        return -1;
    }
    
    int epollfd = epoll_create(MAX_N);
    if (epollfd < 0) {
        perror("epoll_create()");
        exit(1);
    }
    
    struct epoll_event ev, events[MAX_N];
    ev.data.fd = server_listen;
    ev.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen, &ev) < 0) {
        perror("epoll_ctl");
        exit(0);
    }
    
    unsigned long ul = 1;
    ioctl(server_listen, FIONBIO, &ul);
    int nfds;

    for (;;) {
        nfds = epoll_wait(epollfd, events, MAX_N, 10 * 1000);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(1);
        }
        if (nfds == 0) {
            printf("time out!\n");
        }
         
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_listen && events[i].events == EPOLLIN) {
                int new_sock = accept(server_listen, NULL, NULL);
                if (new_sock <  0) {
                    perror("accept");
                    exit(1);
                }
                printf("have client!\n");
                ev.data.fd = new_sock;
                ev.events = EPOLLIN | EPOLLET;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, new_sock, &ev) < 0) {
                    perror("epoll_ctl(new_sock)");
                    nfds += 1;
                    continue;
                }
            } else if (events[i].events & EPOLLIN) {
                char buff[MAX_BUFF] = {0};
                int client_fd = events[i].data.fd;
                struct sockaddr_in addr;
                socklen_t len = sizeof(addr);
                getpeername(client_fd, (struct sockaddr *)&addr, &len);
                memset(buff, 0, sizeof(buff));
                strcpy(all_buff[client_fd], "\0");
                if (recv(client_fd, buff, MAX_BUFF, 0) <= 0) {
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, client_fd, &ev);
                    close(client_fd);
                    nfds--;
                    continue;
                }
                
                printf("%s ---> %s\n", inet_ntoa(addr.sin_addr), buff);
                strcpy(all_buff[client_fd], buff);
                struct epoll_event eul;
                eul.events = EPOLLOUT;
                eul.data.fd = client_fd;
                epoll_ctl(epollfd, EPOLL_CTL_MOD, client_fd, &eul);
            } else if (events[i].events & EPOLLOUT) {
                int client_fd = events[i].data.fd;
                send(client_fd, all_buff[client_fd], strlen(all_buff[client_fd]), 0);
                struct epoll_event eul;
                eul.events = EPOLLIN;
                eul.data.fd = client_fd;
                epoll_ctl(epollfd, EPOLL_CTL_MOD, client_fd, &eul);
                printf("send success!\n");
            }
        }
    }
    close(2);
    close(server_listen);
    return 0;
}
