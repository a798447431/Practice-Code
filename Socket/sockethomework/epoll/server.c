/*************************************************************************
	> File Name: server.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月09日 星期二 19时49分44秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"

#define MAX_EVENTS 40
#define PORT 7777
#define MAX_BUFF 1024

int main() {
    int count = 0, i;
    int maxfds;
    struct epoll_event ev, events[MAX_EVENTS];
    int server_listen, conn_sock, nfds, epollfd;
    int client_fds[MAX_EVENTS];
    char buff[MAX_BUFF] = {0};
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);   
    
    server_listen = socket_create(PORT);
    
    for (int i = 0; i <= MAX_EVENTS; i++) {
        client_fds[i] = -1;
    }
    maxfds = -1;
    
    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(1);
    }

    ev.events = EPOLLIN;
    ev.data.fd = server_listen;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        exit(1);
    }

    for (;;) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(1);
        }

        for (i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_listen) {
                conn_sock = accept(server_listen, (struct sockaddr *)&client_addr, &len);
                if (conn_sock == -1) {
                    perror("accept");
                    exit(1);
                }
                
                for (count = 0; count < MAX_EVENTS; count++) {
                    if (client_fds[count] < 0) {
                        client_fds[count] = conn_sock;
                        break;
                    }
                }

                if (count == MAX_EVENTS) {
                    printf("too many client!\n");
                    exit(1);
                }
                
                if (count > maxfds) {
                    maxfds = count;
                }
                
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    exit(1);
                } 
            }
            else if (events[i].data.fd & EPOLLIN) {
                    int sockfd;
                    sockfd = events[i].data.fd;
                    int recvnum;
                    recvnum = recv(sockfd, buff, MAX_BUFF, 0);
                    if (recvnum <= 0) {
                        for (int j = 0; j <= maxfds; j++) {
                            if (client_fds[j] == sockfd) {
                                client_fds[j] = -1;
                                break;
                            }
                        }
                        if (epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, NULL) == -1) {
                            perror("epoll_ctl: conn_sock");
                            exit(1);
                        }
                        close(sockfd);
                    }
                    printf("%s", buff);
                    ev.events = EPOLLOUT | EPOLLET;
                    ev.data.fd = sockfd;
                    if (epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &ev) == -1) {
                        perror("epoll_ctl: listen_sock");
                        exit(1);
                    }
                }
                else if (events[i].data.fd & EPOLLOUT) {
                    int clientfd = events[i].data.fd;
                    send(clientfd, buff, MAX_BUFF, 0);
                    struct epoll_event eul;
                    eul.events = EPOLLIN;
                    eul.data.fd = clientfd;
                    epoll_ctl(epollfd, EPOLL_CTL_MOD, clientfd, &eul);
                    printf("send success\n");
                }
            }
        }
    close(server_listen);
    close(epollfd);
    return 0;
}
