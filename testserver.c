/*************************************************************************
	> File Name: testserver.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月22日 星期六 12时28分22秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"

int main(int argc, char *argv[]) {
    int server_listen, sockfd, port;
    pid_t pid;
    struct sockaddr_in client;
    socklen_t len;
    if (argc != 2) {
        printf("Usage:./server port\n");
        fflush(stdout);
        exit(1);
    }
    port = atoi(argv[1]);
    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create()");
        exit(1);
    }

    printf("server_listen = %d\n", server_listen);
    len = sizeof(client);
    while (1) {

        if ((sockfd = accept(server_listen, (struct sockaddr *)&client, &len)) < 0) {
            perror("accept()");
            exit(1);
        }
        printf("accept_return = %d\n", sockfd);
        pid = fork();
            if (pid < 0) {
            perror("fork()");
            break;
        }
        if (pid == 0) {
            char buff[512] = {0};
            int recv_num;
            printf("Child!\n");
            close(server_listen);
            if ((recv_num =  recv(sockfd, buff, sizeof(buff), 0)) > 0) {
                printf("Name of Client is : %s \n", buff);
            }
            close(sockfd);
            wait(NULL);
            exit(0);
        }
        wait(NULL);
        close(sockfd);
    }
    return 0;
}
