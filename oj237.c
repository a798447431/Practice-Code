/*************************************************************************
	> File Name: oj237.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月13日 星期四 19时26分24秒
 ************************************************************************/

#include<stdio.h>

int arr[25];
int vis[25];

void output(int k, int m) {
    if (k == m) {
        for (int i = 0; i < m; i++) {
            i == 0 || printf(" ");
            printf("%d", arr[i]);
        }
        printf("\n");
        return ;
    }

    for (int i = 1; i <= m; i++) {
        arr[k] = i;
        if (vis[i]) continue;
        vis[i] = 1;
        output(k + 1, m);
        vis[i] = 0;
    }
    return ;
}

int main() {
    int m;
    scanf("%d", &m);
    output(0, m);
    return 0;
}
