/*************************************************************************
	> File Name: gongxiangneicun.c
	> Author: szt
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月28日 星期日 18时32分33秒
 ************************************************************************/

#include <sys/ipc.h>
#include <sys/shm.h>
#include "common.h"

struct sm_msg {
    char buff[1024] = {0};
    pthread_mutex_t sm_mutex;
    pthread_cond_t sm_ready;
};

int main() {
    key_t key;
    void *shm = NULL;
    char *share_memory = NULL;
    
    pthread_mutexattr_t m_attr;
    pthread_cond_t c_attr;
    
    pthread_mutexattr_t_init(&m_attr);
    pthread_condattr_init(&c_attr);

    pthread_mutexattr_setpshared(&m_attr, PTHREAD_PROCESS_SHARED);
    pthread_condattr_setpshared(&c_attr, PTHREAD_PROCESS_SHARED);
    
    if ((key = ftok(".", 's')) < 0) {
        perror("fail to ftok");
        exit(1);
    }
    printf("%d\n", key);
    int shmid;
    if ((shmid = shmget(key, sizeof(sm_msg), 0666 | IPC_CREAT)) == -1) {
        perror("shmaget");
        exit(1);
    }
    
    if ((shm = shmat(shmid, 0, 0)) < 0) {
        perror("shmat");
        exit(1);
    }

    struct sm_msg *msg = (struct sm_msg *)share_memory;
    
    pthread_mutex_init(&(msg->sm_mutex), &m_attr);
    pthread_cond_init(&(msg->sm_ready), &c_attr);

    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        scanf("%s", shared);
        printf("child: -> %s\n", shared);
    } 
    wait(NULL);
    printf("father-> : %s\n", shared);
    
    /*
    if(shmctl(shmid, IPC_RMID, 0) == -1) {
        printf("shmctl(IPC_RMID) failed\n");
        exit(1);    
    }
    */
    return 0;
}
