/*************************************************************************
	> File Name: test10.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月12日 星期日 14时26分28秒
 ************************************************************************/
       #include <arpa/inet.h>
       #include <stdio.h>
       #include <stdlib.h>

       int
       main(int argc, char *argv[])
       {
           struct in_addr addr;

           if (argc != 2) {
               fprintf(stderr, "%s <dotted-address>\n", argv[0]);
           }

           if (inet_aton(argv[1], &addr) == 0) {
               fprintf(stderr, "Invalid address\n");
           }

           printf("%s\n", inet_ntoa(addr));
       }

