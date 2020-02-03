/*************************************************************************
	> File Name: readlog.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月18日 星期四 19时40分10秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>

#define MAX_BUFF 1024

int get_log_info(char *filename, char *log_word) {
    char buf[MAX_BUFF] = {0};
    int length;
    FILE *open = NULL;
    
    if ((open = fopen(filename, "r")) == NULL) {
        printf("error: %s 文件不存在!\n", filename);
        return -1;
    }
    for (int i = 0; i < MAX_BUFF; i++) {
        fgetc(&buf, open);
    }
    for (int i = 0; i < MAX_BUFF; i++) {
        printf("%c", buf[i]);
    }
    fclose(open);
    return 0;
}

int main() {
    char *filename = "log.txt";
    char *word = "hello";
    get_log_info(filename, word);
    return 0;
}
