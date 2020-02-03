/*************************************************************************
	> File Name: oj250.cpp
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年06月11日 星期二 19时37分53秒
 ************************************************************************/

#include<iostream>
using namespace std;
#define MAX_N 1000000
long long arr[MAX_N + 5];
long long sum[MAX_N + 5];

int main() {
    int n;
    long long total = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%lld", arr + i);
        total += arr[i];
    }
    long long c = total / n, ans = 0;
    sum[0] = arr[0] - c;
    for (int i = 1; i < n; i++) {
        sum[i] = sum[i - 1] + arr[i] - c;
    }
    nth_element(sum, sum + n / 2, sum + n);
    c = sum[n / 2];

    return 0;
}
