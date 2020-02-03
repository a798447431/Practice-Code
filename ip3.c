/*************************************************************************
	> File Name: ip3.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月13日 星期一 00时24分30秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
/*
int *is_ip(char *ip_str) {
    int temp_arr[4];
    if (ip_str != NULL) {
         if(sscanf(ip_str, "%d.%d.%d.%d", &temp_arr[0], &temp_arr[1], &temp_arr[2], &temp_arr[3]) == 4 && temp_arr[0] >= 0 && temp_arr[0] <= 255 && temp_arr[1] >= 0 && temp_arr[1] <= 255 && temp_arr[2] >= 0 && temp_arr[2] <= 255 && temp_arr[3] >= 0 && temp_arr[3] <= 255) {
            for (int i = 0; i < 4; i++) {
                printf("temp_arr[%d] = %d\n", i, temp_arr[i]);
            }
            return temp_arr;
         } else {
            printf("ERROR!\n");
            exit(1);
         }
    } else {
        printf("ERROR!\n");
        exit(1);
    }
}
*/

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("ERROR\n");
        return -1;
    }
    
    else{
        int ip1[4], ip2[4], netmask[4];
        if(sscanf(argv[1], "%d.%d.%d.%d", &ip1[0], &ip1[1], &ip1[2], &ip1[3]) == 4 && ip1[0] >= 0 && ip1[0] <= 255 && ip1[1] >= 0 && ip1[1] <= 255 && ip1[2] >= 0 && ip1[2] <= 255 && ip1[3] >= 0 && ip1[3] <= 255) {
        if(sscanf(argv[2], "%d.%d.%d.%d", &ip2[0], &ip2[1], &ip2[2], &ip2[3]) == 4 && ip2[0] >= 0 && ip2[0] <= 255 && ip2[1] >= 0 && ip2[1] <= 255 && ip2[2] >= 0 && ip2[2] <= 255 && ip2[3] >= 0 && ip2[3] <= 255) {
        if(sscanf(argv[3], "%d.%d.%d.%d", &netmask[0], &netmask[1], &netmask[2], &netmask[3]) == 4 && netmask[0] >= 0 && netmask[0] <= 255 && netmask[1] >= 0 && netmask[1] <= 255 && netmask[2] >= 0 && netmask[2] <= 255 && netmask[3] >= 0 && ip1[3] <= 255) {
                        
        if ((ip1[0]&netmask[0])==(ip2[0]&netmask[0]) && (ip1[1]&netmask[1])==(ip2[1]&netmask[1]) && (ip1[2]&netmask[2])==(ip2[2]&netmask[2]) && (ip1[3]&netmask[3])==(ip2[3]&netmask[3])) {
            printf("Yes\n");
         } 
        else {
            printf("No\n");
            return -1;
         }
            }
        }
    }
        /*
        for (int i = 0; i < 4; i++) {
            printf("ip1[%d] = %d\n", i, ip1[i]);
        }
        for (int i = 0; i < 4; i++) {
            printf("ip2[%d] = %d\n", i, ip2[i]);
        }
        for (int i = 0; i < 4; i++) {
            printf("netmask[%d] = %d\n", i, netmask[i]);
        }

        if ((ip1[0]&netmask[0])==(ip2[0]&netmask[0]) && (ip1[1]&netmask[1])==(ip2[1]&netmask[1]) && (ip1[2]&netmask[2])==(ip2[2]&netmask[2]) && (ip1[3]&netmask[3])==(ip2[3]&netmask[3])) {
            printf("Yes\n");
         } 
        else {
            printf("No\n");
            return -1;
         }
         */

    }
    return 0;
}
