/*************************************************************************
	> File Name: ep08.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年07月02日 星期二 11时37分31秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "008.h"

int main() {
    int64_t ans = 1, zero = 0, maxN = 0;
    for (int i = 0; a[i]; ++i) {
        if (a[i] != '0') {
            ans *= a[i] - '0';
        } else {
            ++zero;
        }
        if (i >= 13) {
            if (a[i - 13] != '0') {
                ans /= a[i - 13] - '0';
            } else {
                --zero;
            }
        }
        if (zero == 0 && ans > maxN) maxN = ans;
    }
    printf("%" PRId64"\n", maxN);
    return 0;
}
