/*************************************************************************
	> File Name: testclient.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月22日 星期六 12时26分47秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"

int main(int argc, char *argv[]) {
    int port, sockfd;
    char ip[20] = {0};
    if (argc != 4) {
        printf("Usage:./client ipaddr port username\n");
        exit(1);
    }
    port = atoi(argv[2]);
    strcpy(ip, argv[1]);
    char *name = argv[3];

    if ((sockfd = socket_connect(ip, port)) < 0) {
        perror("socket_connect()");
        exit(1);
    }

    ssize_t send_num;
    send_num = send(sockfd, name, strlen(name), 0);
    if (send_num < 0) {
        perror("send()");
    }
    printf("Send name with %d bytes\n", send_num);
    printf("scokfd = %d\n", sockfd);

    sleep(100);
    return 0;

}
