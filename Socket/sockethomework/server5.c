/*************************************************************************
	> File Name: server5.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月07日 星期日 15时43分35秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>

int remaxfd(fd_set fds, int maxfd) {
    int new_maxfd = 0;
    for (int i = 0; i <= maxfd; i++) {
        if (FD_ISSET(i, &fds) && i > new_maxfd) {
            new_maxfd = i;
        }
    }
    return new_maxfd;
}

int main(int argc, char *argv[]) {
    int sockfd;
    int port = 8888;
    struct sockaddr_in bind_addr;
    struct timeval tv;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() error");
        exit(1);
    }
    unsigned long ul = 1;
    ioctl(sockfd, FIONBIO, &ul);
    memset(&bind_addr, 0, sizeof(bind_addr));
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port = htons(port);
    bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(sockfd, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) < 0) {
        perror("bind() error");
        exit(1);
    }
    
    if (listen(sockfd, 20) < 0) {
        perror("listen() error");
        exit(1);
    }

    printf("socket_create is done!\n");

    fd_set rset;
    fd_set new_rset;
    int maxfd;
    maxfd = sockfd;
    FD_ZERO(&rset);
    FD_ZERO(&new_rset);
    FD_SET(sockfd, &rset);
    
    int new_sock, res;
    char buff[4096] = {0};
    int recv_length = 0;

    while (1) {
        rset = new_rset;
        maxfd = remaxfd(rset, maxfd);
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        
        res = select(maxfd + 1, &rset, NULL, NULL, &tv);
        if (res < 0) {
            perror("select()");
            exit(1);
        }

        else if (res == 0) {
            printf("chaoshi!\n");
            continue;
        }
        
        for (int i = 0; i <= maxfd; i++) {
            if (FD_ISSET(i, &rset)) {
                if (i == sockfd) {
                    ioctl(new_sock, FIONBIO, &ul);
                    FD_SET(new_sock, &rset);
                    FD_SET(new_sock, &new_rset);
                    new_sock = accept(sockfd, NULL, NULL);
                    if (new_sock < 0) {
                        perror("accept() error");
                        exit(1);
                    }
                    ioctl(new_sock, FIONBIO, &ul);
                    if (new_sock > maxfd) {
                        maxfd = new_sock;
                    }
                    FD_SET(new_sock, &new_rset);
                }
                else {
                    memset(buff, 0, sizeof(buff));
                    if ((recv_length = recv(i, buff, sizeof(buff), 0)) < 0) {
                        perror("recv() error");
                        exit(1);
                    }
                    printf("recv info is %s\n", buff);
                    if (send(i, buff, recv_length, 0) < 0) {
                        perror("send() error");
                        exit(1);
                    }
                    close(new_sock);
                    FD_CLR(i, &new_rset);
                }
            } 
        }
    }
    return 0;
}
