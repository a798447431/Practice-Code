/*************************************************************************
	> File Name: ep34.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月02日 星期二 10时31分04秒
 ************************************************************************/

#include <stdio.h>
#include <inttypes.h>
#define MAX_N 2540160


int fac(int x) {
    if (x == 1) return 1;
    return x * fac(x - 1);
}
/*
void init(int *num) {
    num[0] = 1;
    for (int i = 1; i <= 10; i++) {
        num[i] = num[i - 1] * i;
    }
}
*/

int is_num(int y) {
    int num = y;
    int sum = 0;
    while (y) {
        sum += fac(y % 10);
        y /= 10;
    }
    if (sum == num) return 1;
    return -1;
}

int main() {
    int sum = 0;
 // int *num;
 // init(num);
    for (int i = 3; i <= MAX_N; i++) {
        if (is_num(i) == 1) sum += i;
    }
    printf("%d\n", sum);
    return 0;
}
