/*************************************************************************
	> File Name: select_03.cpp
	> Author: victoria 
	> Mail: 1105847344@qq.com
	> Created Time: 2019年07月08日 星期一 11时31分45秒
 ************************************************************************/

#include <iostream>
#include "common_n.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#define MAX_N 200
#define PORT 8888
using namespace std;
char all_buff[200][1024] = {0};

int main() {
    int listen_socket = socket_create(PORT);
    int ep_fd = epoll_create(MAX_N);
    if (ep_fd < 0) {
        perror("epoll_create");
        exit(1);
    }
    struct epoll_event ev, events[MAX_N];
    ev.data.fd = listen_socket;
    ev.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(ep_fd, EPOLL_CTL_ADD, listen_socket, &ev) < 0) {
        perror("epoll_ctl");
        exit(0);
    }
    unsigned long ul = 1;
    ioctl(listen_socket, FIONBIO, &ul);
    int nfds;
    while (1) {
        nfds = epoll_wait(ep_fd, events, MAX_N, 10*1000);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(1);
        }
        if (nfds == 0) {
            cout << "time out" << endl;
        }
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == listen_socket && events[i].events == EPOLLIN) {
                int new_sock = accept(listen_socket, NULL, NULL);
                if(new_sock < 0) {
                    perror("accept");
                    exit(1);
                }
                cout << "someone comes" << endl;
                ioctl(new_sock, FIONBIO, &ul);
                ev.data.fd = new_sock;
                ev.events = EPOLLIN | EPOLLET;
                if (epoll_ctl(ep_fd, EPOLL_CTL_ADD, new_sock, &ev) < 0){
                    perror("epoll_ctl : new_sock");
                    nfds += 1;
                    continue;
                }
            } else if (events[i].events & EPOLLIN){ //通讯
                char buff[1024] = {0};
                int client_fd = events[i].data.fd;
                struct sockaddr_in addr;
                socklen_t len = sizeof(addr);
                getpeername(client_fd, (struct sockaddr*)&addr, &len);
                memset(buff, 0, sizeof(buff));
                strcpy(all_buff[client_fd], "\0");
                if (recv(client_fd, buff, 1024, 0) <= 0) {
                    cout << "someone leaves" << endl;
                    epoll_ctl(ep_fd, EPOLL_CTL_DEL, client_fd, &ev);
                    close(client_fd);
                    nfds --;
                    continue;
                }
                cout << inet_ntoa(addr.sin_addr) << "--->" << buff << endl;
                strcpy(all_buff[client_fd], buff);
                struct epoll_event eul;
                eul.events = EPOLLOUT;
                eul.data.fd = client_fd;
                epoll_ctl(ep_fd, EPOLL_CTL_MOD, client_fd, &eul);
            } else if (events[i].events & EPOLLOUT) {
                int client_fd = events[i].data.fd;
                send(client_fd, all_buff[client_fd], strlen(all_buff[client_fd]), 0); 
                struct epoll_event eul;
                eul.events = EPOLLIN;
                eul.data.fd = client_fd;
                epoll_ctl(ep_fd, EPOLL_CTL_MOD, client_fd, &eul);
                cout << "send success" <<endl;
            }
        }
    }
    close(2);
    close(listen_socket);
    return 0;
}


