/*************************************************************************
	> File Name: test.cpp
	> Author: suziteng
	> Mail: 253604653@qq.com
	> Created Time: 2019年05月18日 星期六 16时44分33秒
 ************************************************************************/
#include<gtest/gtest.h>
int add(int a,int b){
    return a+b;
}
TEST(testCase,test0){
    EXPECT_EQ(add(2,3),4);
}
int main(int argc,char **argv){
  testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
