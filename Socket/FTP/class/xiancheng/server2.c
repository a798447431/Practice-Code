/*************************************************************************
	> File Name: server.c
	> Author:
	> Mail:
	> Created Time: Thu 20 Jun 2019 18:48:03 CST
 ************************************************************************/

#include <stdio.h>
#include "common.h"

#define MAX_SIZE 1024

int port, server_listen, server_fd, pid, server_file_listen, server_file;
char buff[MAX_SIZE] = {0};
char pathname[100] = {0};

struct sockaddr_in client_addr;
socklen_t len = sizeof(client_addr);

char ip[20] = {0};

void recv_filename() {
    strcpy(ip, inet_ntoa(client_addr.sin_addr));
    printf("%s\n", ip);

    if (NULL == opendir(ip)) {
        mkdir(ip, 0755);
    }

    close(server_listen);
    if (recv(server_fd, buff, sizeof(buff), 0) > 0) {
        printf("Recv Filename = %s\n", buff);
    }
    sprintf(pathname, "%s/%s", ip, buff);
    printf("accept success %d\n", server_fd);
    int ack = 1;
    send(server_fd, &ack, sizeof(ack), 0);
}

void recv_file() {
    FILE *fp = fopen(pathname, "w");
    while (1) {
        int size;
        memset(buff, 0, sizeof(buff));
        if ((size = recv(server_file, buff, sizeof(buff), 0)) > 0) {
            fwrite(buff, 1, size, fp);
        }
        if (size <= 0) {
            close(server_file);
            break;
        }
    }
    fclose(fp);
    close(server_fd); 
    exit(0);
}

int main(int argc, char *argv[]) {
//    int port, server_listen, server_fd, pid, server_file_listen, server_file;
//    char buff[MAX_SIZE] = {0};
//    char pathname[100] = {0};
    if (argc != 2) {
        printf("Usage:./server port\n");
        exit(1);
    }
    port = atoi(argv[1]);
    if ((server_listen = socket_create(port)) < 0) {
        printf("scoket_create: %s\n", strerror(errno));
        exit(1);
    }
    if ((server_file_listen = socket_create(6666)) < 0) {
        printf("scoket_create: %s\n", strerror(errno));
        exit(1);
    }
    while (1) {
    //   struct sockaddr_in client_addr;
    //    socklen_t len = sizeof(client_addr);
    //    char ip[20] = {0};
        pthread_t fn, f;
        if ((server_fd = accept(server_listen, (struct sockaddr *)&client_addr, &len)) < 0 ){
            printf("accept: %s\n", strerror(errno));
            exit(1);
        }
        pthread_create(&fn, NULL, recv_filename, NULL);
        if ((server_file = accept(server_file_listen, NULL, NULL)) < 0) {
            perror("server_file_listen");
        }
        pthread_create(&f, NULL, recv_file, NULL);
        pthread_join(f, NULL);
        pthread_join(fn, NULL);
    }
    return 0;
}
