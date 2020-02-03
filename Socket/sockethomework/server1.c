/*************************************************************************
	> File Name: server1.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月06日 星期六 22时48分54秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"

int main() {
    int sockfd;
    if (sockfd = socket_create(8888) < 0) {
        printf("socket_create() error!\n");
        return -1;
    }
    int new_sock;
    while (1) {
        accept(sockfd, NULL, NULL);
        int i;
        send(new_sock, &i, sizeof(i), 0);
        close(new_sock);
    }
    close(sockfd);
    return 0;
}
