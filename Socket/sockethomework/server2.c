/*************************************************************************
	> File Name: server2.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月07日 星期日 10时28分35秒
 ************************************************************************/
#include "common.h"
#include <stdio.h>

int main() {
    int port = 9999;
    int listen_socket;
    listen_socket = socket_create(port);
    sleep(10);
    int new_socket; 
    if( ( new_socket = accept(listen_socket, NULL, NULL))<0){
        perror("accept");
        exit(0);
    }
    int i = 1024;
    if((send(new_socket, &i, sizeof(i), 0)) <= 0) {
        perror("send()");
        exit(0);
    } else {
        printf("send: %d\n", i);
    }
    close(new_socket);
    close(listen_socket);
    return 0;
}
