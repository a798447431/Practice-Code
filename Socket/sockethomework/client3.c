/*************************************************************************
	> File Name: client2.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月07日 星期日 14时26分39秒
 ************************************************************************/

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int socket_connect_nonblock(char *ip, int port, long timeout) {
    int sockfd, ret;
    struct sockaddr_in server;
    struct timeval tm;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);
    
   // printf("connect to %s:%d\n", ip, port);

    unsigned long ul = 1;
    ioctl(sockfd, FIONBIO, &ul);
    
    fd_set set;

    FD_ZERO(&set);
    FD_SET(sockfd, &set);
    
    tm.tv_sec = 0;
    tm.tv_usec = timeout;
    
    int error;
    int len;
    len = sizeof(error);

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        if (select(sockfd + 1, NULL, &set, NULL, &tm) > 0) {
            if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len) < 0) {
                perror("getsockopt() error");
                ret = -1;
            }
            if (error == 0) {
                ret = 0;
            } else {
                ret = -1;
            }
        }
    }
    
    close(sockfd);
    return ret;
}

int main() {
    char ip[20] = {0};
    int ret;
    for (int i = 1; i < 255; i++) {
        sprintf(ip, "192.168.2.%d", i);
        ret = socket_connect_nonblock(ip, 22, 100000);
        if (ret == 0) printf("192.168.2.%d can connect!\n", i);
    }
    return 0;
}
