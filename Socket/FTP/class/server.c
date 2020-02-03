/*************************************************************************
	> File Name: server.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月20日 星期四 18时48分48秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"
#define MAX_SIZE 1024

int main(int argc, char *argv[]) {
    int port, server_listen, server_fd, pid, server_file_listen, server_file;
    char buff[MAX_SIZE] = {0};
    char pathname[100] = {0};
    if (argc != 2) {
        printf("Usage:./server_port\n");
        exit(1);
    }
    
    port = atoi(argv[1]);

    if ((server_listen = socket_create(port)) < 0) {
        printf("socket_create: %s\n", strerror(errno));
        exit(1);
    }
    
    if ((server_file_listen = socket_create(8888)) < 0) {
        printf("socket_create: %s\n", strerror(errno));
        exit(1);
    }

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        char ip[20] = {0};
        
        if ((server_fd = accept(server_listen, (struct sockaddr *)&client_addr, &len)) < 0) {
            printf("accept: %s\n", strerror(errno));
            exit(1);
        } 

        strcpy(ip, inet_ntoa(client_addr.sin_addr));
        printf("%s\n", ip);
        
        if (NULL == opendir(ip)) {
            mkdir(ip, 0755);
        }

        if ((pid = fork()) < 0) {
            perror("fork() error");
            exit(1);
        } 

        if (pid = 0) {
            //close(server_listen);
            if (recv(server_fd, buff, sizeof(buff), 0) > 0) {
                printf("Recv Filename = %s\n", buff);
            }
            sprintf(pathname, "%s%s", ip, buff);
            printf("accept success %d\n", server_fd);
            int ack = 1;
            send(server_fd, &ack, sizeof(ack), 0);
            
            if ((server_file = accept(server_file_listen, NULL, NULL)) < 0) {
                perror("server_file_listen");
            }
            
            FILE *fp = fopen(pathname, "w");
            memset(buff, 0, sizeof(buff));
            int recv_num;
            while (recv_num = recv(server_file, buff, sizeof(buff), 0)) {
                if (fwrite(buff, sizeof(char), recv_num, fp) < recv_num) {
                    printf("File:\t%s Write Failed\n", pathname);
                    break; 
                }
            }
            printf("File:\t%s Write Success!\n", pathname);
            fclose(fp);
        }
        close(server_fd);
    }
    close(server_listen);
    return 0;
}
