/*************************************************************************
	> File Name: master1.h
	> Author: szt
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月25日 星期四 20时13分56秒
 ************************************************************************/

#include "common.h"
#define BUFFSIZE 1024
#define MAXCLIENT 10000

typedef struct Node {
    struct sockaddr_in client_addr; //存储ip地址的结构体
    int node_sock; //套接字描述符
    struct Node *next;
} Node, *LinkedList;

struct HEART {
    LinkedList *head;
    int ins;
    int *sum;
    long timeout;
};

struct WORK {
    LinkedList head;
    int index;
    int ctlport;
};

int find_min(int *sum, int ins);
int insert(LinkedList head, Node *node);
void print_linkedlist(LinkedList head);
void *heartbeat_all(void *arg);
void *work(void *arg);
void *do_work(void *arg);
int check_connect(struct sockaddr_in, long timeout);
int do_event(struct sockaddr_in);
int socket_connect1(struct sockaddr_in);
int do_epoll(int server_listen, LinkedList *linkedlist, int *sum, int ins, int heartport);

int find_min(int *sum, int ins) {
    int sub = 0;
    for (int i = 0; i < ins; i++) {
        if (*(sum + i) < *(sum + sub)) {
            sub = i;
        }
    }
    return sub;
}

int insert(LinkedList head, Node *node) {
    Node *p = head;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = node;
    return 0;
}

void print_linkedlist(LinkedList head) {
    Node *p = head;
    int cnt = 0;
    while (p->next != NULL) {
        printf("<%d> %s\n", cnt++, inet_ntoa(p->client_addr.sin_addr));
        p = p->next;
    }
    return ;
}

int check_connect(struct sockaddr_in addr, long timeout) {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 0;
    }
    int error = -1, len, ret = 0;
    struct timeval tv;
    unsigned long ul = 1;
    tv.tv_sec = 0;
    tv.tv_usec = timeout;
    len = sizeof(int);
    ioctl(sockfd, FIONBIO, &ul);

    fd_set set;
    FD_ZERO(&set);
    FD_SET(sockfd, &set);

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        if (select(sockfd + 1, NULL, &set, NULL, &tv) > 0) {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
            if (error == 0) {
                ret = 1;
            } else {
                ret = 0;
            }
        }
    }
    close(sockfd);
    return ret;
}

void *heartbeat_all(void *arg) {
    struct HEART *heart = (struct HEART *)arg;
    while (1) {
        for (int i = 0; i < heart->ins; i++) {
            Node *p = heart->head[i];
            while (p->next != NULL) {
                if (!check_connect(p->next->client_addr, heart->timeout)) {
                    printf("%s:%d \033[31mdeleting\033[0m ... \n", inet_ntoa(p->next->client_addr.sin_addr), ntohs(p->next->client_addr.sin_port));
                    Node *q;
                    q = p->next;
                    p->next =p->next->next;
                    free(q);
                    heart->sum[i]--;
                } else {
                    printf("%s:%d \033[32monline\033[0m ... \n", inet_ntoa(p->next->client_addr.sin_addr), ntohs(p->next->client_addr.sin_port));
                    p = p->next;
                }
            }
        }
        sleep(5);
    }
    return NULL;
}
/*
void *work(void *arg) {
    struct WORK *inarg = (struct WORK *)arg;
    char log[50] = {0};
    sprintf(log, "./%d.log", inarg->index);
    while (1) {
        FILE *file = fopen(log, "w");
        Node *p = inarg->head;

        while (p->next != NULL) {
            fprintf(file, "%s:%d\n", inet_ntoa(p->next->client_addr.sin_addr), ntohs(p->next->client_addr.sin_port));
            do_event(p->next->client_addr);
            p = p->next;
        }
        fclose(file);
        sleep(5);
    }
    return NULL;
}
*/
int socket_connect1(struct sockaddr_in addr) {
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return -1;
    }
    return sockfd;
}
/*
int do_event(struct sockaddr_in addr) {
    int sockfd;
    char buff[BUFFSIZE] = {0};
    addr.sin_port = htons(CTLPORT);
    if ((sockfd = socket_connect1(addr)) < 0) {
        perror("socket_connect1");
        return -1;
    }

    send(sockfd, "Hello", strlen("hello"), 0);

    if (recv(sockfd, buff, BUFFSIZE, 0) <= 0) {
        close(sockfd);
        return 0;
    }
    close(sockfd);
    printf("%s\n", buff);
    return 0;
}
*/
void *do_work(void *arg) {
    struct WORK *inarg = (struct WORK *)arg;
    char log[50] = {0};
    sprintf(log, "./%d.log", inarg->index);
    FILE *file = fopen(log, "w");
    int epollfd = epoll_create(MAXCLIENT);
    struct epoll_event events[MAXCLIENT], ev;

    while (1) {
        Node *p = inarg->head;

        while (p->next != NULL) {
            int enquire_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (enquire_fd < 0) {
                perror("socket to enquire_fd");
                continue;
            }
            unsigned long ul = 1;
            ioctl(enquire_fd, FIONBIO, &ul);
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(inarg->ctlport);
            addr.sin_addr = p->next->client_addr.sin_addr;
            if (connect(enquire_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("sock_connect");
                continue;
            }
            fprintf(file, "%s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
            
            ev.events = EPOLLOUT;
            ev.data.fd = enquire_fd;
            epoll_ctl(epollfd, EPOLL_CTL_ADD, enquire_fd, &ev);
            p->node_sock = enquire_fd;
            p = p->next;
        }
        
        int reval = epoll_wait(epollfd, events, MAXCLIENT, 3000);
        if (reval < 0) {
            perror("epoll wait");
            fclose(file);
            return NULL;
        } else if (reval == 0) {
            continue;
        }
        for (int i = 0; i < reval; i++) {
            if (events[i].events & EPOLLOUT) {
                int clientfd = events[i].data.fd;
                unsigned long ul = 0;
                ioctl(clientfd, FIONBIO, &ul);
                struct epoll_event em;
                char buff[BUFFSIZE] = {0};
                send(clientfd, "hello", strlen("hello"), 0);
                int k = recv(clientfd, buff, BUFFSIZE, 0);
                if (k > 0) {
                    printf("%s\n", buff);
                }
                em.data.fd = clientfd;
                epoll_ctl(epollfd, EPOLL_CTL_DEL, clientfd, &em);
                close(clientfd);
            }
        }
        fclose(file);
        sleep(5);
    }
}

int do_epoll(int server_listen, LinkedList *linkedlist, int *sum, int ins, int heartport) {
    unsigned long ul = 1;
    int nfds;
    ioctl(server_listen, FIONBIO, &ul);
    int epollfd = epoll_create(MAXCLIENT);
    if (epollfd < 0) {
        perror("epoll_create");
        exit(1);
    }
    struct epoll_event events[MAXCLIENT], ev;
    ev.data.fd = server_listen;
    ev.events = EPOLLIN;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen, &ev) < 0) {
        perror("epoll_ctl");
        exit(0);
    }
    while (1) {
        nfds = epoll_wait(epollfd, events, MAXCLIENT, -1);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(1);
        } else if (nfds == 0) {
            continue;
        }
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_listen && events[i].events & EPOLLIN) {
                struct sockaddr_in addr;
                socklen_t len = sizeof(addr);
                int newfd = accept(server_listen, (struct sockaddr *)&addr, &len);
                if (newfd < 0) {
                    perror("accept");
                    exit(1);
                }
                int sub = find_min(sum, ins);
                Node *p = (Node *)malloc(sizeof(Node));
                addr.sin_port = htons(heartport);
                p->client_addr = addr;
                p->node_sock = newfd;
                p->next = NULL;
                insert(linkedlist[sub], p);
                sum[sub]++;
                printf("insert success!\n");
            }
        }
    }
}
