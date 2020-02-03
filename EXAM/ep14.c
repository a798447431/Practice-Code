/*************************************************************************
	> File Name: ep14.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月02日 星期二 14时18分03秒
 ************************************************************************/

#include <stdio.h>
#include <inttypes.h>
#define MAX_N 1000000
#define MAX 50000000

int arr[MAX + 5] = {0};

int get_length(int64_t x) {
    if (x == 1) return 1;
    if (x <= MAX && arr[x] != 0) return arr[x];
    int ans;
    if (x & 1) ans = get_length(x * 3 + 1) + 1;
    else ans = get_length(x >> 1) + 1;
    if (x < MAX) arr[x] = ans; 
    return ans;
}

int main() {
    int ans, count = 0;
    for (int i = 1; i < MAX_N; i++) {
        int temp_length = get_length(i);
        if (temp_length > count) {
            ans = i;
            count = temp_length;
        }
    }
    printf("%d(%d)\n", ans, count);
    return 0;
}
