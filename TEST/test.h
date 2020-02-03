/*************************************************************************
	> File Name: test.h
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月18日 星期六 19时51分13秒
 ************************************************************************/

#ifndef _TEST_H
#define _TEST_H

#include <stdio.h>
#include <stdlib.h>

#define _COLOR(a, b) "\033[" #b "m" a "\033[0m"
#define COLOR(a, b) _COLOR(a, b)

#define RED(a) COLOR(a, 31)
#define GREEN(a) COLOR(a, 32)
#define YELLOW(a) COLOR(a, 33)
#define BLUE(a) COLOR(a, 34)

#define _FuncName(a, b) a##b
#define FuncName(a, b) _FuncName(a, b)
#define STR(a) #a

#define TEST(a, b)  \
void FuncName(a, FuncName(_, FuncName(b, FuncName(_, Test))))();\
__attribute__((constructor)) void FuncName(add_Test, __COUNTER__)() {\
    pFuncData p = (pFuncData)malloc(sizeof(FuncData));\
    p->func = FuncName(a, FuncName(_, FuncName(b, FuncName(_, Test))));\
    p->name_part1 = STR(a);\
    p->name_part2 = STR(b);\
    p->next = head;\
    head = p;\
}\
    void FuncName(a, FuncName(_, FuncName(b, FuncName(_, Test))))()

#define TYPE_STR(a) _Generic((a), \
    int : "%d", \
    double : "%lf", \
    float : "%f", \
    char : "%c", \
    char * : "%s" \
)
#define P(a, color)  {\
    char frm[100];\
    sprintf(frm, color("%s"), TYPE_STR(a));\
    printf(frm, a);\
}

#define EXPECT(a, b, c) { \
    __typeof(a) __temp_a = a, __temp_b = b;\
    if (__temp_a c __temp_b) test_success();\
    else { \
        test_failed(__FILE__, __LINE__, "expect " #a " " #c " " #b); \
        printf(YELLOW(" actual : ")); \
        P(__temp_a, YELLOW); \
        P(" vs ", YELLOW); \
        P(__temp_b, YELLOW); \
        printf("\n\n"); \
    }\
    printf("%s %s %s %s\n", #a, #c, #b, __temp_a c __temp_b ? "True" : "False");\
}\

#define EXPECT_EQ(a, b) EXPECT(a, b, ==)
#define EXPECT_NE(a, b) EXPECT(a, b, !=)
#define EXPECT_GT(a, b) EXPECT(a, b, >)
#define EXPECT_LT(a, b) EXPECT(a, b, <)
#define EXPECT_LE(a, b) EXPECT(a, b, <=)
#define EXPECT_GE(a, b) EXPECT(a, b, >=)

typedef void (*FuncType)();

typedef struct FuncData{
    FuncType func;
    const char *name_part1;
    const char *name_part2;
    struct FuncData *next;
} FuncData, *pFuncData;

typedef struct test_info{
    int succ;
    int fail;
} test_info;

static pFuncData head = NULL;
static struct test_info info; 

void clear_info() {
    info.succ = info.fail = 0;
}

void test_success() {
    info.succ += 1;
}

void test_failed(const char *file, int line_no, const char *meg) {
    printf(YELLOW("%s:%d: Failure\n"), file, line_no);
    printf(YELLOW("%s"), meg);
    info.fail += 1;
}

void output_info() {
    double rate = 1.0 * info.succ / (info.succ + info.fail);
    const char *output_str = NULL;
    char output_frm[100];
    if (info.fail == 0) output_str = BLUE("%6.2lf%%");
    else output_str = RED("%6.2lf%%");
    sprintf(output_frm, "[ %s ] total : %%3d  expect : %%3d\n", output_str);
    printf(output_frm, rate * 100, info.succ + info.fail, info.succ);
    return ;
}

int RUN_ALL_TEST() {
    pFuncData p = head;
    while (p != NULL) {
        clear_info();
        printf(GREEN( "[  RUN  ]" ));
        printf("[%s %s]\n", p->name_part1, p->name_part2);
        p->func();
        output_info();
        p = p->next;
    }
    return 0;
}

#endif
