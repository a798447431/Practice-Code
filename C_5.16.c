/*************************************************************************
	> File Name: C_5.16.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月18日 星期六 11时14分53秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char **env) {
    char workdir[256];
    char pathname[256];
    for (int i = 0; env[i] != NULL; i++) {
        if (strcmp(env[i], "USER=szt") == 0) {
            if (strcmp(argv[0], "./haizeix") == 0) {
                printf("hello haizei!\n");
                break;
            }
        }
        
    }
    return 0;
}
