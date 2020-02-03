/*************************************************************************
	> File Name: p1025.c
	> Author: 
	> Mail: 
	> Created Time: 2019年04月07日 星期日 10时42分24秒
 ************************************************************************/

#include<stdio.h>

int method(int n, int k) {
    if (n < k) return 0;
    if (k == 1) return 1;
    return method(n - 1, k - 1) + method(n - k, k);
}


int main() {
    int n, k; //整数n，分为k份
    scanf("%d%d", &n, &k);
    printf("%d\n", method(n, k));

    return 0;
}
