/*************************************************************************
	> File Name: ep03.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月02日 星期二 09时39分52秒
 ************************************************************************/

#include <stdio.h>
#include <inttypes.h>
#define MAX_N 600851475143

int main() {
    int64_t max = MAX_N;
    int64_t getnum;
    for (int64_t i = 2; i * i < MAX_N; i++) {
        while (max % i == 0) {
            max /= i;
            getnum = i;
        }
    }
    if (max != 1) getnum = max;
    printf("%" PRId64"\n", getnum);
    return 0;
}
