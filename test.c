/*************************************************************************
	> File Name: test.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年04月27日 星期六 18时45分13秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char *argv[]) {
    DIR *dir = NULL;
    struct passwd *pwd;
    struct dirent *dp;
    struct stat *buf = malloc(sizeof(struct stat));
    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
    }
    while ((dp = readdir(dir)) != NULL) {   
        if (stat(dp->d_name, buf) < 0) {
            perror("stat");
            return -1;
        }
        struct passwd *pw = getpwuid(buf->st_uid);
        struct group *gp = getgrgid(buf->st_gid);
        printf("%d\t%d\t%s\t%s\t%d\t %s\n", buf->st_mode, buf->st_nlink, pw->pw_name, gp->gr_name, buf->st_size, dp->d_name);
    }
    closedir(dir);
    return 0;
}
