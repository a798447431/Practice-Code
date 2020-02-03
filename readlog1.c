/*************************************************************************
	> File Name: readlog1.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月18日 星期四 20时42分29秒
 ************************************************************************/

#include "readlog1.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./test key!\n");
        exit(1);
    }
    
    char *file = argv[1];
    char *key = argv[2];
    char value[100] = {0};
    
    if (get_conf_value(file, key, value) < 0) {
        printf("x!\n");
        exit(1);
    } else {
        printf("value = %s!\n", value);
    }
    return 0;
}
