/*************************************************************************
	> File Name: class_ls.c
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月11日 星期六 14时42分05秒
 ************************************************************************/

#include <stdio.h> 
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#include <string.h>
#include <grp.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define LEN 512

//0:ls
//1:ls -a
//2:ls -l
//3:ls -al

int ls_opt = 0;
char work_dir[LEN];

int cmp_name(const void* _a, const void* _b) {
    char *a = (char *)_a;
    char *b = (char *)_b;
    return strcmp(a, b);
}

void do_ls(char *);
void size_window(char file_name[][LEN], int cnt, int *row, int col);
void show_file_info(char *filename, struct stat *info);
void do_stat(char *);
void mode_to_letters(mode_t, char *);
char *uid_to_name(uid_t);
char *gid_to_name(git_t);

int main(int argc, char *argv[]) {
    int opt;
    getcwd(work_dir, LEN);
    while ((opt = getopt(argc, argv, "al")) != -1 ) {
        switch(opt) {
            case 'a':
                printf("a\n");
                ls_opt++;
                break;
            case 'l':
                printf("l\n");
                ls_opt+=2;
                break;
            default:
                printf("opt :%c unknown\n", opt);
                exit(1);
        }
    }
    argc -= (optind - 1);
    argv += (optind - 1);
    printf("argv[%d] = %s\n", argc - 1, *(argv + 1));
    if (argc == 1) {
        do_ls(".");
    } else {
        while (--argc) {
            do_ls(*(++argv));
        }
    }
    return 0;
}

void do_ls(char *dirname) { 
    printf("%s:\n", dirname);
    chdir(work_dir);
    DIR *dir_ptr = NULL;
    struct dirent *direntp;
    char file_name[1024][LEN] = {0};
    if ((dir_ptr = opendir(dirname)) == NULL) {
        perror("opendir");
    } else {
        int cnt = 0;
        while ((direntp = readdir(dir_ptr)) != NULL) {
            strcpy(file_name[cnt++], direntp->d_name);
            printf("%s\n", direntp->d_name);
        }
        qsort(file_name, cnt, LEN, cmp_name);
/*        for (int i = 0; i < cnt; i++) {
            printf("file_name[%d] = %s\n", i, file_name[i]);
        }
*/
        if (is_opt >= 2) {
            for (int i = 0, i < cnt, i++) {
                do_stat(filename[i]);
            }
        } else if (is_opt == 0) {
                    
            show_files();
        } 
    }
}

void size_window(char file_name[][LEN], int cnt, int *row, int col) {
    struct winsize size;
    if (isatty(STDOUT_FILENO) == 0) {
        exit(1);
    }
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
        perror("ioctl")
        exit(1);
    }
    printf("size_row = %d, size_col = %d\n", size.ws_row, size.ws_col);
}

void do_stat(char *filename) {
    struct stat info;
    if (stat(filename, &info) == -1) {
        perror(filename);
    } else {
        show_file_info(filename, &info);
    }
}

void show_file_info(char *filename, struct stat *info) {
    char modestr[11] = "----------";
    char *uid_to_name, *gid_to_name;
    mode_to_letters(info->st_mode, modestr);
    printf("%s ", modestr);
    printf("%4d ", (int)info->st_nlink);
    printf("%8s ", uid_to_name(info->st_uid));
    printf("%8s ", gid_to_name(info->st_gid));
    printf("%8ld", (long)info->st_size);
    printf("%.12s", 4 + ctime(&info->st_mtime));
    printf("%s\n", filename);
}

void mode_to_letters(mode_t, char *str) {
    if (S_ISDIR(mode)) str[0] = 'd';
    if (S_ISCHR(mode)) str[0] = 'c';
    if (S_ISBLK(mode)) str[0] = 'b';
    if (S_ISSOCK(mode)) str[0] = 's';
    if (S_ISFIFO(mode)) str[0] = 'p';
    if (S_ISLNK(mode)) str[0] = 'l';

    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';


    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';

    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';

}

char *uid_to_name(uid_t uid) {
    struct passwd *pw_ptr;
    static char numstr[10] = {0};
    if ((pw_ptr = getpwuid(uid)) == NULL) {
        sprintf(numstr, "%d", uid);
        return numstr;
    } else {
        return pw_ptr->pw_name;
    }
}
char *gid_to_name(gid_t gid) {
    struct group *gp_ptr;
    static char numstr[10] = {0};
    if ((gp_ptr = getgrgid(gid)) == NULL) {
        sprintf(numstr, "%d", gid);
        return numstr;
    } else {
        return gp_ptr->gr_name;
    }
}
