/*************************************************************************
	> File Name: p1057.c
	> Author: 
	> Mail: 
	> Created Time: 2019年04月07日 星期日 13时07分01秒
 ************************************************************************/

#include<stdio.h>
#define max 30

int array[max + 5][max + 5] = {0};
int ans[max + 5][max + 5] = {0};
int m, n;

int method(int m, int location) {
    if (m == 0) return location == 0 ? 1 : 0; 
    if (ans[m][location] == 1) return array[m][location];
    array[m][location] = method(m - 1, (location + 1) % n) + method(m - 1, (location + n - 1) % n), ans[m][location] = 1;
    return array[m][location];
}


int main() {
    scanf("%d%d", &n, &m);
    printf("%d\n", method(m, 0));
    return 0;
}
