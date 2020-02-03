/*************************************************************************
	> File Name: ep05.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月02日 星期二 10时52分49秒
 ************************************************************************/

#include <stdio.h>
#include <inttypes.h>

int gcd(int64_t a, int64_t b) {
    if (!b) return a;
    return gcd(b, a % b);
}

int main() {
    int64_t ans = 1;
    for (int64_t i = 1; i <= 20; i++) {
        ans *= i / gcd(i, ans);
    }
    printf("%" PRId64"\n", ans);
    return 0;
}
