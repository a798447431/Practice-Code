/*************************************************************************
	> File Name: common.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月20日 星期六 14时34分06秒
 ************************************************************************/

#include "common.h"

int socket_create(int port) {
    int socket_fd;
    int yes = 1;
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        close(socket_fd);
        perror("setsockopt() error\n");
        return -1;
    }

    if (bind(socket_fd, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
        return -1;
    }

    if (listen(socket_fd, 1000) == -1) {
        return -1;
    }

    return socket_fd;
}

int socket_connect(char *IP, int port) {
    int socket_fd;
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }

    struct sockaddr_in client_add;
    client_add.sin_family = AF_INET;
    client_add.sin_port = htons(port);
    client_add.sin_addr.s_addr = inet_addr(IP);
    bzero(&(client_add.sin_zero), 8);
    
    if (connect(socket_fd, (struct sockaddr *)&client_add, sizeof(socket_fd)) == -1) {
        close(socket_fd);
        return -1;
    }

    return socket_fd;
}

int socket_noblock_connect(char*IP,int port){
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("make socket:");
        return -1;
    }

    fcntl(sockfd,F_SETFL,O_NDELAY);

    struct sockaddr_in their_add;
    their_add.sin_family = AF_INET;
    their_add.sin_port = htons(port);
    their_add.sin_addr.s_addr=inet_addr(IP);
    bzero(&(their_add.sin_zero),8);

    connect(sockfd,(struct sockaddr*)&their_add,sizeof(struct sockaddr));
    
    if(errno != 115)return -1;
    
    return sockfd;
}


int get_conf_value(char *file, char *key, char *value) {
    FILE *fp = NULL;
    char *line = NULL, *substr = NULL;
    size_t n = 0, len = 0;
    ssize_t read;

    if (key == NULL || value == NULL) {
        printf("error\n");
        return -1;
    }

    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Open config file error!\n");
        return -1;
    }

    while ((read = getline(&line, &n, fp)) != -1) {
        substr = strstr(line, key);
        if (substr == NULL) continue;
        else {
            len = strlen(key);
            if (line[len] == '=') {
                strncpy(value, &line[len + 1], (int)read - len - 2);
                break;
            } else {
                continue;
            }
        }
    }
    if (substr == NULL) {
        printf("%s Not found in %s!\n", key, file);
        fclose(fp);
        free(line);
        return -1;
    }
    fclose(fp);
    free(line);
    return 0;
}

