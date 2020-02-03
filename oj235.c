/*************************************************************************
	> File Name: oj235.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月13日 星期四 18时09分13秒
 ************************************************************************/

#include<stdio.h>

void output(int ind, int n, int k, int *buff) {
    if (ind == n) {
        return ;
    }
    for (int i = n; i >= ind + 1; i--) {
        buff[k] = i;
        output(i, n, k + 1, buff);
        for (int j = 0; j <= k; j++) {
            j == 0 || printf(" ");
            printf("%d", buff[j]);
        }
        printf("\n");
        //output(i, n, k + 1, buff);
    }
    return ;
}

int arr[25];

int main() {
    int n;
    scanf("%d", &n);
    output(0, n, 0, arr);
    return 0;
}
