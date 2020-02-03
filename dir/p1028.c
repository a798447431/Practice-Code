/*************************************************************************
	> File Name: p1028.c
	> Author: 
	> Mail: 
	> Created Time: 2019年04月07日 星期日 11时15分22秒
 ************************************************************************/

#include<stdio.h>
#define max 1000
int skeep[max + 5] = {0};

int method(int n) {
    if (n == 1) return 1;
    if (skeep[n] != 0) return skeep[n];
    if (n % 2) {
        return skeep[n] = method(n - 1);
    } else {
        return skeep[n] = method(n - 1) + method(n / 2);
    }
}
    

int main() {
    int n;
    scanf("%d", &n);
    printf("%d\n", method(n));
    return 0;
}
