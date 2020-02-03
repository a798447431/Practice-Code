/*************************************************************************
	> File Name: ep06.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月02日 星期二 10时02分43秒
 ************************************************************************/

#include <stdio.h>

int main() {
    int num1 = 5050 * 5050;
    int num2;
    int num3 = (2 * (100 * 100 * 100) + 3 * (100 * 100) + 100) / 6;
    for (int i = 1; i <= 100; i++) {
        num2 += i * i; 
    }
    printf("%d\n", num1 - num2);
    printf("%d\n", num1 - num3);
    return 0;
}
