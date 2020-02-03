#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "link.h"
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/select.h>
LinkedList linked;
pthread_mutex_t mut;
unsigned long ul = 1;

void *do_heart() {
    char file[50], key[50], value[50], buff[1024];
    strcpy(file, "server_conf.log");
    strcpy(key, "HeartPort");
    get_conf_value(file, key, value);
    int port = atoi(value);
    Node *p = linked->next;
    while (p) {
        int sockfd;
        struct timeval tm;
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            continue;
        }
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(p->name);
        ioctl (sockfd, FIONBIO, &ul);
        fd_set wfds;
        FD_ZERO(&wfds);
        FD_SET(sockfd, &wfds);
        tm.tv_sec = 0;
        tm.tv_usec = 3000000;
        int error = -1;
        int len = sizeof(int);
        if(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            if (select(sockfd + 1, NULL, &wfds, NULL, &tm) > 0) {
                if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *) &len) < 0) {
                    perror("getsockopt");
                }
                if (error == 0) {
                    printf("%s-->connect success\n", p->name);
                    memset(buff, 0, sizeof(buff));
                    strcpy(buff, "hello vic");
                    send(sockfd, buff, strlen(buff), 0);
                    close(sockfd);
                } else {
                    printf("%s-->is not online\n", p->name);
                    //linked = delete_node(linked, p->name);
                }
            } else {
                printf("%s-->is not online\n", p->name);
                //linked = delete_node(linked, p->name);
            }
        }
        if (p->next == NULL) {
            p = linked->next;
        } else {
            p = p->next;
        }
        sleep(1);
    }
}

void init_list() {
    char file[50], key[50], value[50];
    int ips, ipe;
    char ip[20];
    strcpy(file, "server_conf.log");
    strcpy(key, "_Ip");
    get_conf_value(file, key, value);
    strcpy(ip, value);
    memset(value, 0, sizeof(value));
    strcpy(key, "IpS");
    get_conf_value(file, key, value);
    ips = atoi(value);
    memset(value, 0, sizeof(value));
    strcpy(key, "IpEnd");
    get_conf_value(file, key, value);
    ipe = atoi(value);
    for (int i = ips; i <= ipe; i++) {
        char tmp_buff[50];
        sprintf(tmp_buff, "%s%d",ip, i);
        Node *node = CreateNode(tmp_buff);
        linked = insert(linked, node);
    } 
}

int main() {
    //链表初始化
    linked = (LinkedList) malloc (sizeof(Node));
    init_list();
    //抛出子线程
    pthread_t pth_heart;
    pthread_create(&pth_heart, NULL, do_heart, NULL);
    //pthread_join(pth_heart, NULL);
    //建立监听套接字
    char file[50], key[50], value[50];
    strcpy(file, "server_conf.log");
    strcpy(key, "ListenPort");
    int port = atoi(key);
    int listen_fd = socket_create(port);
    struct timeval tm;

    fd_set rfds;
    ioctl (listen_fd, FIONBIO, &ul);
    while (1) {
        FD_ZERO(&rfds);
        FD_SET(listen_fd, &rfds);
        tm.tv_sec = 5;
        tm.tv_usec = 0;
        Node *q = linked->next;
        int reval = select(listen_fd+1, &rfds, NULL, NULL, &tm);
        if (reval < 0) {
            perror("Error on select");
            continue;
        }
        if (reval == 0) {
            continue;
        }
        if (FD_ISSET(listen_fd, &rfds)) {
            struct sockaddr_in addr;
            socklen_t len = sizeof(addr); 
            int newfd = accept(listen_fd, (struct sockaddr*)&addr, &len);
            if (newfd < 0) {
                perror("Error accept on [listen_fd]");
                exit(1);
            }
            Node *node = CreateNode(inet_ntoa(addr.sin_addr));
            linked = insert(linked, node);
        }
    }
    return 0;
}
