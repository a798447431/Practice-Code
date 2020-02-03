/*************************************************************************
	> File Name: client.c
	> Author: victoria 
	> Mail: 1105847344@qq.com
	> Created Time: 2019年07月24日 星期三 19时29分28秒
 ************************************************************************/

#include "client.h"
#include <pthread.h>
#define PORT 8521
#define BUFFSIZE 1024
int temp = 1;

void sigfunc(int signo) {
   temp = 0; 
}

void do_con() {
    signal(SIGINT, sigfunc);
    int port = 8733;
    char ip[20] = {0};
    strcpy(ip, "192.168.2.58");
    while (temp) {
        int sockfd = socket_connect(ip, port);
        if (sockfd < 0) {
            close(sockfd);
            sleep(3);
            continue;
        }

        while(temp) {
            sleep(1);
        }
        close(sockfd);
    }
}


int main() {
    int listenfd;
    if ((listenfd = socket_create(PORT)) < 0) {
        perror("socket_create");
        exit(1);
    }
   // pthread_t pth_con;
   // pthread_create(&pth_con, NULL, do_con, NULL);
    pid_t pid = fork();
    if (pid > 0) {
    while (temp) {
    signal(SIGINT, sigfunc);
        int newfd;
        if ((newfd = accept(listenfd, NULL, NULL)) < 0) {
            perror("accept");
            continue;
        }
        char buff[BUFFSIZE] = {0};
        int k = recv(newfd, buff, BUFFSIZE, 0);
        if (k < 0) {
            perror("recv");
            close(newfd);
            continue;
        }
        printf("%s\n", buff);
        char buff1[BUFFSIZE] = {0};
        sprintf(buff1, "clinet:%s", buff);
        send(newfd, buff1, strlen(buff1), 0);
        close(newfd);
    }
    close(listenfd);
    wait(&pid);
    }
    if (pid == 0) {
        do_con();
        exit(0);
    }
    return 0;
}
