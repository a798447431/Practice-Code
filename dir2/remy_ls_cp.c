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

#define MAX_LENGTH 100
#define LS 0
#define LS_AL 1

int len_leave = MAX_LENGTH;
int len_max;

int cmp_filename(const void *a, const void *b){
    char *s1 = (char *)a;
    char *s2 = (char *)b;
    return strcmp(s1, s2);
}

void print_info(struct stat *buf) {
    
    struct passwd *pw = getpwuid(buf->st_uid);
    struct group *gp = getgrgid(buf->st_gid);
    char timebuf[64] = {0};

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

void simple_info(char *filename) {
/*    int len;
    if (len_leave < len_max) {
        printf("\n");
        len_leave = MAX_LENGTH;
    }
    len = strlen(filename);
    len = len_max - len;
*/
    printf("%s\n", filename);
/*
    for (int i = 0; i < len; i++) {
        printf(" ");
    }
    printf(" ");
    len_leave -= (len_max + 2);
*/
}


void show_info(int sign, char *pathname) {
    int m, n;
    struct stat *buf = malloc(sizeof(struct stat));
    char filename[MAX_LENGTH];
    //从路径中截取文件名,否则直接输出数组中文件名时文件名前面会有路径信息，参考了linuxC编程实战一书
    for (m = 0, n = 0; m < strlen(pathname); m++) {
        if (pathname[m] == '/') {
            n = 0;
            continue;
        }
        filename[n++] = pathname[m];
    }
    filename[n] = '\0';
	
    if (stat(pathname, buf) == -1) {
		perror("stat");
	}
    
    //判断是ls还是ls -al, sign为0是ls,为1是ls -al
    switch(sign) {
        case LS:
            if (filename[0] != '.') {
				simple_info(filename);
			} break;
        
        case LS_AL:
        	print_info(buf);
			printf("%s\n", filename);
            break;
        
        default:
            break;
    }        
}


int dir_info(int sign_param, char *dirname) { 
    DIR *dir = NULL;
    struct dirent *dp;
    struct stat *buf = malloc(sizeof(struct stat));
    int count = 0;
    char sort_filenames[MAX_LENGTH][MAX_LENGTH];
    
    //先获取文件总数与最长文件名的长度
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("opendir");
    }
    
    while ((dp = readdir(dir)) != NULL) {
        if (len_max < strlen(dp->d_name)) {
            len_max =  strlen(dp->d_name);
        }
        count++;
    }
    closedir(dir);

    dir = opendir(dirname);
    for (int i = 0; i < count; i++) {
        dp = readdir(dir);
        if (dp == NULL) {
            perror("readdir");
        }
        strcpy (sort_filenames[i], dirname);
        strcat (sort_filenames[i], "/");
        strcat (sort_filenames[i], dp->d_name);
    }
    //排序
    qsort(sort_filenames, count, sizeof(sort_filenames[0]), cmp_filename);
    for (int i = 0; i < count; i++) {
        show_info(sign_param, sort_filenames[i]);
    }
    closedir(dir);
}

void filename_info(char *filename) {
    struct stat *buf = malloc(sizeof(struct stat));  
    if (stat(filename, buf) < 0) {
        perror("stat");
    }
    print_info(buf);
    printf("%s\n", filename);
}

int main(int argc, char *argv[]) {
    struct stat *buf = malloc(sizeof(struct stat));  
    char *dirname;
    char *filename;
    int	sign_param = LS;
    if (argc == 1) {
        dirname = ".";
        dir_info(sign_param, dirname);
    } else{
        for (int i = 1; i < argc; i++) {
            stat(argv[i], buf);
            if(S_IFDIR & buf->st_mode){ 
                dirname = argv[i];
                dir_info(sign_param, dirname);
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
