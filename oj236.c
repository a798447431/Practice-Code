/*************************************************************************
	> File Name: oj236.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月13日 星期四 19时03分08秒
 ************************************************************************/

#include<stdio.h>

int arr[25];

void output(int k, int m, int ind, int n) {
    if (k == m) {
        for (int i = 0; i < m; i++) {
            i == 0 || printf(" "); 
            printf("%d", arr[i]);
        }
        printf("\n");
        return ;
    }
    for (int i = ind + 1; i <= n; i++) {
        arr[k] = i;
        output(k + 1, m, i, n);
    }
    return ;
}

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    output(0, m, 0, n);
    return 0;
}
