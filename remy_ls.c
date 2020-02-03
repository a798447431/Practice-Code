/*************************************************************************
	> File Name: my_ls.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月09日 星期四 11时42分13秒
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
#include <sys/ioctl.h>

#define MAX_LENGTH 256
#define LS 0
#define LS_A 1
#define LS_L 2
#define LS_AL 3

int len_max;
int ls_flag = 0;
int ls_opt = 0;

static int get_screen_width(int *colum)                         
{
	struct winsize size;

	if(isatty(STDOUT_FILENO)==0)
	{
		return -1;  
	}

	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size)<0)
	{
		return -1;  
	}

	*colum = size.ws_col;
	return 0;
}

int cmp_filename(const void *a, const void *b){
    char *s1 = (char *)a;
    char *s2 = (char *)b;
    return strncasecmp(s1, s2, MAX_LENGTH);
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
    
    printf(" ");
    
    printf("%4ld", buf->st_nlink);

    printf(" %-8s", pw->pw_name);

    printf("%-8s", gp->gr_name);

    printf("%6ld", (int)(buf->st_size));
    
    ctime_r(&(buf->st_mtime), timebuf);
    if(timebuf[0] != 0)
        timebuf[strlen(timebuf)-1] = 0; /*去掉'\n'*/
    printf(" %-20.20s", timebuf);

}

void simple_info(char *filename) {
    int col_width = 0;
    get_screen_width(&col_width);
    int one_line_items = 0;
    one_line_items = col_width / (len_max + 2);
    printf("%-*s", len_max + 2, filename);
    ls_flag++;
    if (ls_flag == one_line_items) {
        printf("\n");
        ls_flag = 0;
    }
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
        exit(1);
	}
    
    //判断是ls还是ls -al, sign为0是ls,为1是ls -al
    switch(sign) {
        case LS:
           if (filename[0] != '.') {
				simple_info(filename);
            } break;
    
        case LS_A:
            simple_info(filename);
            break;

        case LS_L:
            if (filename[0] != '.') {
                print_info(buf);
			    printf(" %-s\n", filename);
            } break;

        case LS_AL:
            print_info(buf);
			printf(" %-s\n", filename);
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
        exit(1);
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
            exit(1);
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

//ls时末尾换行
void linefeed(int sign_param) {
    if (sign_param == LS || sign_param == LS_A) {
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
/*
    struct stat *buf = malloc(sizeof(struct stat));  
    char *dirname;
    char *filename;
    int	sign = LS;
    int flag = 0;
    
    //如果没有任何参数，以ls方式打印当前目录
    if (argc == 1) {
        dirname = ".";
        dir_info(sign, dirname);
        linefeed(sign);
        return 0;
    }
    
    //判断是否输入了-al
    if (strcmp(argv[1], "-al") == 0) {
        sign |= LS_AL;
        flag = 1;
    }
    
    //如果输入-al后没有任何参数，ls -al打印当前目录
    if (argc == 2 && flag == 1) {
        dirname = ".";
        dir_info(sign, dirname);
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        //　当第一个参数为-al时，跳过
        if (i == 1 && flag == 1) {
            continue;
        }
        if(stat(argv[i], buf) == -1) {
            perror("stat");
            exit(1);
        }
        if(S_IFDIR & buf->st_mode){
            dirname = argv[i];
            dir_info(sign, dirname);
            linefeed(sign);
            if (i < argc - 1) printf("\n");
        }
        else if(S_IFREG & buf->st_mode){
            dirname = argv[i];
            show_info(sign, dirname);
            linefeed(sign);
            if (i < argc - 1) printf("\n");
        }
    }
*/
    struct stat *buf = malloc(sizeof(struct stat));  
    char *dirname;
    char *filename;
    int	sign = LS;
    int opt;
    while((opt = getopt(argc, argv, "al")) != -1) {
        switch (opt){
            case 'a':
                ls_opt++;
                sign = ls_opt;
                break;
            case 'l':
                ls_opt += 2;
                sign = ls_opt;
                break;
            default:
                exit(1);
        }
    }
//  printf("argc = %d, optind = %d\n",argc, optind);

    argc -= (optind - 1);
    argv += (optind - 1);
//  printf("argc = %d, optind = %d\n", argc, optind);
    if (argc == 1) {
        dirname = ".";
        dir_info(sign, dirname);
        linefeed(sign);
        return 0;
    }else {
        for (int i = 1; i < argc; i++) {
            if(stat(argv[i], buf) == -1) {
                perror("stat");
                exit(1);
            }
            if(S_IFDIR & buf->st_mode){
                dirname = argv[i];
                dir_info(sign, dirname);
                linefeed(sign);
                if (i < argc - 1) printf("\n");
            }
            else if(S_IFREG & buf->st_mode){
                dirname = argv[i];
                show_info(sign, dirname);
                linefeed(sign);
                if (i < argc - 1) printf("\n");
            }
        }
    }
    return 0;
}

