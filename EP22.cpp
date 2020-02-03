/*************************************************************************
	> File Name: EP22.cpp
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月24日 星期一 21时38分48秒
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <inttypes.h>
#include <algorithm>

int main() {
    int64_t i = 0, len, sum, total = 0;
    while (nameList[i++].length() != 0);
    std::sort(nameList, nameList + i - 1);
    i = 0;
    while (nameList[i].length() != 0) {
        sum = 0;
        for (int j = 0; j < nameList[i].length(); j++) {
            sum += nameList[i][j] - 'A' + 1;
        }
        total += sum * (++i);
    }
    printf("%" PRId64"\n", total);
    return 0;
}
