/*************************************************************************
	> File Name: gongxiangneicun.c
	> Author: szt
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月28日 星期日 18时32分33秒
 ************************************************************************/

#include <sys/ipc.h>
#include <sys/shm.h>
#include "common.h"
#define PORT 8888

int main() {
    int sock_listen, newsock, clientfd;
    char ip[20] = "192.168.2.58";
    key_t key;
    void *shm = NULL;
    char * shared;
    if ((key = ftok(".", 's')) < 0) {
        perror("fail to ftok");
        exit(1);
    }
    printf("%d\n", key);
    int shmid;
    if ((shmid = shmget(key, 4096, 0666 | IPC_CREAT)) < 0) {
        perror("shmaget");
        exit(1);
    }
    
    if ((shm = shmat(shmid, 0, 0)) < 0) {
        perror("shmat");
        exit(1);
    }
    shared = (char *)shm;
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        sleep(1);
        if ((clientfd = socket_connect(PORT, ip)) < 0) {
            perror("socket_connect");
            exit(1);
        }
        while (1) {
            scanf("%s", shared);
          //  printf("child: -> %s\n", shared);
            send(clientfd, shared, strlen(shared), 0);
        }
        close(clientfd);
    } 
    
    if ((sock_listen = socket_create(PORT)) < 0) { 
        printf("socket_create error!\n");
        exit(1);
    }

    if ((newsock = accept(sock_listen, NULL, NULL)) < 0) {
        perror("accpet");
        exit(1);
    }

    while (1) {
        sleep(1);
        recv(newsock, shared, sizeof(shared), 0);
        printf("father-> : %s\n", shared);
        memset(shared, 0, sizeof(shared));
    }
    
    /*
    if(shmctl(shmid, IPC_RMID, 0) == -1) {
        printf("shmctl(IPC_RMID) failed\n");
        exit(1);    
    }
    */
    return 0;
}
