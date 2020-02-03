/*************************************************************************
	> File Name: client.c
	> Author: victoria 
	> Mail: 1105847344@qq.com
	> Created Time: 2019年07月24日 星期三 19时29分28秒
 ************************************************************************/

#include "common.h"
#define BUFFSIZE 1024

void do_con(char *ip, int port) {
    while (1) {
        int sockfd = socket_connect(ip, port);
        if (sockfd < 0) {
            close(sockfd);
            sleep(3);
            continue;
        }
        sleep(100);
        close(sockfd);
    }
}

void recv_heart(int port) {
    int sockfd = socket_create(port);
    if (sockfd < 0) {
        perror("Error bind on heartPort");
        return ;
    }
    while (1) {
        int newfd = accept(sockfd, NULL, NULL) ;
        /*
        printf("-❤- ");
        fflush(stdout);
        */
    } 
    close(sockfd);
}

int main() {
    int listenfd;
    int heartPort, ctlPort, loadPort;
    char tmp[20]={0};
    char *config = "./client_conf.log";
    char ip[20]={0};
    
    get_conf_value(config, "HeartPort", tmp);
    heartPort = atoi(tmp);
    
    memset(tmp, 0, sizeof(tmp));
    get_conf_value(config, "IP", tmp);
    strcpy(ip, tmp);
    
    memset(tmp, 0, sizeof(tmp));
    get_conf_value(config, "CtlPort", tmp);
    ctlPort = atoi(tmp);
    
    memset(tmp, 0, sizeof(tmp));
    get_conf_value(config, "LoadPort", tmp);
    loadPort = atoi(tmp);
    printf("HeartPort : %d\nCtlPort : %d\nLoadPort : %d\n", heartPort, ctlPort, loadPort);
    if ((listenfd = socket_create(ctlPort)) < 0) {
        perror("socket_create on listenfd");
        exit(1);
    }
   // pthread_t pth_con;
   // pthread_create(&pth_con, NULL, do_con, NULL);
    pid_t pid = fork();
    if (pid > 0) {
    while (1) {
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
        pid_t npid = fork();
        if (npid > 0) {  
            do_con(ip, loadPort);
            wait(&npid);
            exit(0);
        }
        if (npid == 0) {
            recv_heart(heartPort);
            exit(0);
        }
    }
    return 0;
}
