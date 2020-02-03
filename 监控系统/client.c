
#include "client.h"

int main() {
    char ip[20] = "192.168.2.58";
    int listenfd;
    int Port = 8732;
    if ((listenfd = socket_create(Port)) < 0) {
        perror("socket_create");
        exit(1);
    }
    pid_t pid = fork();
    if (pid > 0) {
        while (1) {
            int newfd;
            if ((newfd = accept(listenfd, NULL, NULL)) < 0) {
                perror("accept");
                continue;
            }
            char buff[BUFFSIZE] = {0};
            char buff1[BUFFSIZE] = {0};
            if (recv(newfd, buff, BUFFSIZE, 0) > 0){
                printf("%s\n", buff);
                sprintf(buff1, "client: %s ", buff);
                send(newfd, buff1, strlen(buff1), 0);
                close(newfd);
            } else {
                close(newfd);
                continue;
            }
        }
        close(listenfd);
        wait(&pid);
    }
    if (pid == 0) {
        
    }
    return 0;
}
