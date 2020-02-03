/*************************************************************************
	> File Name: readlog1.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月18日 星期四 20时19分22秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int get_conf_value(char *file, char *key, char *value) {
    FILE *fp = NULL;
    char *line = NULL, *substr = NULL;
    size_t n = 0, len = 0;
    ssize_t read;

    if (key == NULL || value == NULL) {
        printf("error\n");
        return -1;
    }
    
    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Open config file error!\n");
        return -1;
    }
    
    while ((read = getline(&line, &n, fp)) != -1) {
        substr = strstr(line, key);
        if (substr == NULL) continue;
        else {
            len = strlen(key);
            if (line[len] == '=') {
                strncpy(value, &line[len + 1], (int)read - len - 2);
                break;
            } else {
                continue;
            }
        }
    }
    if (substr == NULL) {
        printf("%s Not found in %s!\n", key, file);
        close(fp);
        free(line);
        return -1;
    }
    close(fp);
    free(line);
    return 0;
}


