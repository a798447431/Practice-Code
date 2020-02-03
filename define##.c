/*************************************************************************
	> File Name: define##.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月18日 星期六 20时31分18秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

typedef void (*FuncType)();


typedef struct FuncData{
    FuncType func;
    struct FuncData *next;
} FuncData, *pFuncData;


void test_is_prime_func_Test();
int is_prime(int);
void add_Test();

int main() {
    pFuncData p = head;
    
    FuncType func;
    int count = 0; 
    while (p != NULL) {
        p->func();
        count++;
    }
    printf("%d\n", count);
    return 0;
}

void test_is_prime_func_Test(){
    printf("%s\n", is_prime(2) == 0 ? "Yes" : "No");
    printf("%s\n", is_prime(-2) == 0 ? "Yes" : "No");
    printf("%s\n", is_prime(15) == 0 ? "Yes" : "No");
    printf("%s\n", is_prime(9973) == 1 ? "Yes" : "No");
}

int is_prime(int x) {
    if (x <= 1) return 0;
    for (int i = 2; i * i <= x; i++) {
        if (x % i == 0) return 0;
    }
    return 1;
}

