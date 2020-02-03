/*************************************************************************
	> File Name: client.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月20日 星期四 18时24分56秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"

#define MAX_SIZE 1024

void sendfile(int sockfd, char *path) {
    FILE *fp = fopen(path, "r");
    int num_read;
    char data[MAX_SIZE] = {0};
    while (1) {
        memset(data, 0, MAX_SIZE);
        num_read = fread(data, 1, MAX_SIZE, fp);
        if (send(sockfd, data, strlen(data), 0) < 0) {
            printf("send %s\n", strerror(errno));
            fclose(fp);
            return ;
        }
        if (num_read == 0) break;
    }
    fclose(fp);
}


int main(int argc, char *argv[]) {
    int sockfd, port, sock_file, file_port;
    file_port = 6666;
    char filename[50] = {0};
    if (argc != 4) {
        printf("Usage:./client_ip_port filename\n");
        exit(1);
    }
    char *path = argv[3];
    port = atoi(argv[2]);
    char *ip = argv[1];
    char *p;

    if (access(path, R_OK) != 0) {
        printf("access: %s\n", strerror(errno));
        exit(2);
    }
    
    strcpy(filename, (p = strrchr(path, '/')) ? p + 1 : path);
    printf("%s\n", filename);
    
    if ((sockfd = socket_connect(port, ip)) < 0) {
        printf("Error in socket_connect: %s\n", strerror(errno));
        exit(1);
    }
    
    if (send(sockfd, filename, strlen(filename), 0) < 0) {
        printf("send: %s\n", strerror(errno));
    }
    
    int ack;
    if (recv(sockfd, &ack, sizeof(ack), 0) <= 0) {
        perror("recv ack");
    }
    
    if (ack == 1) {
        if ((sock_file = socket_connect(file_port, ip)) < 0) {
            perror("socket_file connect");
            exit(1);
        }
        printf("connect success!\n");
        sendfile(sock_file, path);
        close(sock_file);
    }

    close(sockfd);
    return 0;
}


