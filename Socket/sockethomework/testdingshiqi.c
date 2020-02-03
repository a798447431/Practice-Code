/*************************************************************************
	> File Name: testdingshiqi.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月09日 星期二 18时07分34秒
 ************************************************************************/

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int my_pause(struct timeval tv) {
    int retval;

    retval = select(0, NULL, NULL, NULL, &tv);

    if (retval == -1) {
        printf("wrong!\n");
        return -1;
    }
    else if (retval == 0) {
        
    }

    return retval; 
}

int main() {
    struct timeval tv;
    long sec = 0;
    scanf("%ld", &sec);
    
    tv.tv_sec = sec;
    tv.tv_usec = 0;
    
    my_pause(tv);

    return 0;
}
