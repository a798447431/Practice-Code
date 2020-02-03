/*************************************************************************
	> File Name: common.h
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月20日 星期四 18时26分07秒
 ************************************************************************/
#include <stdio.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdarg.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>

int socket_connect(int port, char *host);
int socket_create(int port);
int socket_connect_nonblock(char *ip, int port, long timeout);
int get_conf_value(char *file, char *key, char *value);
int socket_connect1(struct sockaddr_in addr);

