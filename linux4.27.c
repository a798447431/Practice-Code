/*************************************************************************
	> File Name: test.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年04月27日 星期六 18时45分13秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define MAX_LENGTH 256

void print_info(struct stat *buf) {
    
    struct passwd *pw = getpwuid(buf->st_uid);
    struct group *gp = getgrgid(buf->st_gid);
    char timebuf[128] = {0};

    if(S_ISREG(buf->st_mode))
        putchar('-');
    else if(S_ISDIR(buf->st_mode))
        putchar('d');
    else if(S_ISCHR(buf->st_mode))
        putchar('c');
    else if(S_ISBLK(buf->st_mode))
        putchar('b');
    else if(S_ISFIFO(buf->st_mode))
        putchar('f');
    else if(S_ISLNK(buf->st_mode))
        putchar('l');
    else if(S_ISSOCK(buf->st_mode))
        putchar('s');
    else
        putchar('?');
    
    // 用户权限
    printf(buf->st_mode & S_IRUSR ? "r":"-");
    printf(buf->st_mode & S_IWUSR ? "w":"-");
    printf(buf->st_mode & S_IXUSR ? "x":"-");

    // 组权限
    printf(buf->st_mode & S_IRGRP ? "r":"-");
    printf(buf->st_mode & S_IWGRP ? "w":"-");
    printf(buf->st_mode & S_IXGRP ? "x":"-");

    // 其它权限
    printf(buf->st_mode & S_IROTH ? "r":"-");
    printf(buf->st_mode & S_IWOTH ? "w":"-");
    printf(buf->st_mode & S_IXOTH ? "x":"-");

    printf("\t");
    
    printf("%d\t", buf->st_nlink);

    printf("%s\t", pw->pw_name);

    printf("%s\t", gp->gr_name);

    printf("%d\t", (int)(buf->st_size));
    
    ctime_r(&(buf->st_mtime), timebuf);
    if(timebuf[0] != 0)
        timebuf[strlen(timebuf)-1] = 0; /*去掉'\n'*/
    printf("%-20.20s\t", timebuf);

}

int dir_info(char *dirname) { 
    DIR *dir = NULL;
    struct dirent *dp;
    struct stat *buf = malloc(sizeof(struct stat));  
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }
    //chdir(dirname);
    while ((dp = readdir(dir)) != NULL) {
        char complete_d_name[MAX_LENGTH];  // 文件的完整路径
        strcpy (complete_d_name,dirname);
        strcat (complete_d_name,"/");
        strcat (complete_d_name,dp->d_name);
        if (stat(complete_d_name, buf) < 0) {
            perror("stat");
            return 1;
        }
        print_info(buf);
        printf("%s\n", dp->d_name);
    }
    closedir(dir);
}

int filename_info(char *filename) {
    struct stat *buf = malloc(sizeof(struct stat));  
    if (stat(filename, buf) < 0) {
        perror("stat");
        return 1;
    }
    print_info(buf);
    printf("%s\n", filename);
}

int main(int argc, char *argv[]) {
    struct stat *buf = malloc(sizeof(struct stat));  
    char *dirname;
    char *filename;
    if (argc == 1) {
        dirname = ".";
        dir_info(dirname);
    } else{
        for (int i = 1; i < argc; i++) {
            stat(argv[i], buf);
            if(S_IFDIR & buf->st_mode){ 
                dirname = argv[i];
                dir_info(dirname);
                if (i < argc - 1) printf("\n");
            }
            else if(S_IFREG & buf->st_mode){ 
                filename = argv[i];
                filename_info(filename);
                if (i < argc - 1) printf("\n");
            }
            else {
                printf("Not files or directories!\n");
            }
        }
    }
    return 0;
}
