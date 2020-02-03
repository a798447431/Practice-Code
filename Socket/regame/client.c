/*************************************************************************
	> File Name: client.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月26日 星期三 21时07分31秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"

int main(int argc, char *argv[]) {
    int clientfd, client_connect;
    struct passwd *pwd;
	pwd = getpwuid(getuid());
	char username[20] = {0};
	strcpy(username, pwd->pw_name);
    
    printf("%s\n", username);

    if (argc != 4) {
        printf("参数错误: 参数分别是ip, port, team！\n");
        exit(1);
    }
    char *ip = argv[1];
    int port = atoi(argv[2]);
    int term = atoi(argv[3]);

    if ((clientfd = socket_connect(port, ip)) < 0) {
        printf("Error in socket_connect: %s\n", strerror(errno));
        exit(1);
    }
    
    if (send(clientfd, username, strlen(username), 0) < 0) {
        perror("send() error");
        return -1;
    }
    
    return 0;
}
