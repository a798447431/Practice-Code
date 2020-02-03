/*************************************************************************
	> File Name: ep30.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月02日 星期二 10时21分28秒
 ************************************************************************/

#include <stdio.h>
#include <math.h>

#define MAX_N 354294

int is_num(int n) {
    int temp = 0, num = n;
    int sum = 0;
    while (n) {
        temp = n % 10;
        sum += pow(temp, 5);
        n /= 10;
    }
    if (sum == num) return 1;
    return -1;
}

int main() {
    int sum = 0;
    for (int i = 2; i <= MAX_N; i++) {
        if (is_num(i) == 1) sum += i;
    }
    printf("%d\n", sum);
    return 0;
}
