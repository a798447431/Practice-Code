/*************************************************************************
	> File Name: ep28.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月02日 星期二 10时08分25秒
 ************************************************************************/

#include <stdio.h>

int main() {
    int sum = 1, len = 3;
    while (len <= 1001) {
        sum += 4 * len * len - 6 * len + 6;
        len += 2;
    }
    printf("%d\n", sum);
    return 0;
}
