/*************************************************************************
	> File Name: quanpailei.cpp
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月25日 星期二 22时39分59秒
 ************************************************************************/

#include <iostream>
#include <algorithm>
using namespace std;

int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

int main() {
    int x, y;
    scanf("%d%d", &x, &y);
    
    int tmp = y - 1;

    while (tmp--) {
        next_permutation(arr, arr + x);
    }
    
    for (int i = 0; i < x; i++) {
        printf("%d", arr[i]);
    }
    printf("\n");
    return 0;
}
