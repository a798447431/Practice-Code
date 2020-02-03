/*************************************************************************
	> File Name: oj240.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月13日 星期四 20时03分54秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_N 729

char g[MAX_N + 5][MAX_N + 5];

void output(int n, int i, int j) {
    if (n == 1) {
        g[i][j] = "X";
        return ;
    }

    int l = (int)pow(3, n - 2);

    output(n - 1, i, j);
    output(n - 1, i, j + 2 * l);
    output(n - 1, i + l, j + l);
    output(n - 1, i + 2 * l, j);
    output(n - 1, i + 2 * l, j + 2 * l);
    return ;
}

int main() {
    int n;
    while (scanf("%d", &n)) {
        if (n == -1) break;
        output(n, 0, 0);
        int len = (int)pow(3, n - 1);
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < len; j++) {
                if (g[i][j] == "X") printf("X");
                else printf(" ");
            }
            printf("\n");
        }
        printf("-\n");
    }
    return 0;
}
