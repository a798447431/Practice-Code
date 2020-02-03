/*************************************************************************
	> File Name: common.h
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月20日 星期六 14时34分25秒
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H
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

int server_socket_create(int port, int listen_num);
int socket_connect(char *IP, int port);
int get_conf_value(char *file, char *key, char *value);

#endif
