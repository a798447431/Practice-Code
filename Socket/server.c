/*************************************************************************
	> File Name: server.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月31日 星期五 19时50分48秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

int main() {
    int sockfd;   
    struct sockaddr_in my_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() error");
        return -1;
    }
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(8888);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t socklen = sizeof(struct sockaddr);
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
        perror("bind() error");
        return -1;
    }
    
    if (listen(sockfd, 20) < 0) {
        perror("listen() error");
        return -1;
    }
    int new_sock;
    pid_t pid;
    struct sockaddr_in peer_addr;
    while ((new_sock = accept(sockfd, (struct sockaddr *)&peer_addr, &socklen)) != -1) {
        pid = fork();

        if (pid == -1) {
            perror("fork() error");
            return -1;
        }
        if (pid == 0) {
            close(sockfd);
            char *ip;
            ip = inet_ntoa(peer_addr.sin_addr);
            char buff[4096] = {0};
            recv(new_sock, buff, sizeof(buff), 0);
            printf("%s:%s\n", buff, ip);
            close(new_sock);
            exit(0);
        }
        else {
            close(new_sock);
        }
    }
    close(sockfd);
    return 0;
}
