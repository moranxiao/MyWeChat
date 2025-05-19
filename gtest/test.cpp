#include <gtest/gtest.h>
#include <iostream>

using namespace std;

int abs(int x)
{
    return x >= 0 ? x : -x; 
}

TEST(功能测试,测试用例1)
{
    ASSERT_EQ(abs(10),10);
    ASSERT_NE(abs(20),-20);
}

TEST(功能测试,测试用例2)
{
    EXPECT_EQ(abs(-10),-10);
    EXPECT_NE(abs(-10),-10);
}


int main(int argc,char* argv[])
{
    //初始化测试用例
    testing::InitGoogleTest();
    //运行所有测试用例
    return RUN_ALL_TESTS();
}
