/*************************************************************************
	> File Name: ip.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月12日 星期日 21时58分20秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int64_t str_to_int(const char *str) {
    int64_t temp = 0;
    const char *ptr = str;  
    if (*str == '-' || *str == '+') {
           str++;
       }
    while(*str != 0) {
        if ((*str < '0') || (*str > '9')) {
              break;
           }
           temp = temp * 10 + (*str - '0'); 
           str++;      
       }
    return temp;
}

int64_t is_ip(char *ip_str) {
    int64_t result = 0;
    char temp_str[31] = {"0"};
    int a, b, c, d;
    if (ip_str != NULL) {
         if(sscanf(ip_str, "%d.%d.%d.%d", &a, &b, &c, &d) == 4 && a >= 0 && a <= 255 && b >= 0 && b <= 255 && c >= 0 && c <= 255 && d >= 0 && d <= 255) {
             sprintf(temp_str, "%03d%03d%03d%03d", a, b, c, d);
             result = str_to_int(temp_str);
             return result;
        }
    } else {
        printf("ERROR\n");
        return -1;
    }
}

int64_t main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("ERROR!\n");
        return 1;
    } else {
        int64_t ip1 = is_ip(argv[1]);
        int64_t ip2 = is_ip(argv[2]);
        int64_t netmask = is_ip(argv[3]);
  /*      printf("ip1 = %d, ip2 = %d, netmask = %d, ip1 & netmask = %d, ip2 & netmask = %d\n", ip1, ip2, netmask, (ip1 & netmask), (ip2 & netmask));
 */
        printf("%" PRId64"\n", ip1);
        printf("%" PRId64"\n", ip2);
        printf("%" PRId64"\n", netmask);
        printf("%" PRId64"\n", ip1 & netmask);
        printf("%" PRId64"\n", ip2 & netmask);
        if (ip1 != -1 && ip2 != -1 && netmask != -1) {
            if ((ip1 & netmask) == (ip2 & netmask)) {
                printf("Yes\n");
            } else {
                printf("No\n");
            }
        } else {
            printf("ERROR\n");
        }
    }
    return 0;
}

