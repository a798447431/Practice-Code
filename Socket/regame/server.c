/*************************************************************************
	> File Name: server.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月26日 星期三 20时35分20秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"
#define MAX_N 1024

int main(int argc, char *argv[]) {
    int server_listen, serverfd;
    char username[20] = {0};
    char buff[MAX_N + 5] = {0};
    if (argc != 2) {
        printf("参数error: 缺少port参数！\n");
        exit(1);
    }
    int port = atoi(argv[1]);

    if ((server_listen = socket_create(port)) < 0) {
        printf("socket_create: %s\n", strerror(errno));
        exit(1);
    }
    
    for ( ; ; ) {
        struct sockaddr_in server_addr;
        socklen_t len = sizeof(server_addr);
        serverfd = accept(server_listen, (struct sockaddr *)&server_addr, &len);
        recv(serverfd, username, sizeof(username), 0);
        printf("%s", username);
    }

    return 0;
}
