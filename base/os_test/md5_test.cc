// Author: zhangguoqiang01 <80176975@qq.com>
#include "gtest/gtest.h"
#include "os/md5.h"
TEST(Md5Test, TestCommon)
{
    std::string raw_string_a = "123456";
    std::string raw_string_b = "123446";
    std::string raw_string_a_string_res = md5sum(raw_string_a.c_str(), strlen(raw_string_a.c_str()));
    std::string raw_string_b_string_res = md5sum(raw_string_b.c_str(), strlen(raw_string_b.c_str()));
    printf("input:[%s], output:[%s]\n", raw_string_a.c_str(), raw_string_a_string_res.c_str());
    printf("input:[%s], output:[%s]\n", raw_string_b.c_str(), raw_string_b_string_res.c_str());
    EXPECT_EQ(32, strlen(raw_string_a_string_res.c_str()));
    EXPECT_EQ(32, strlen(raw_string_b_string_res.c_str()));
    EXPECT_STRNE(raw_string_a_string_res.c_str(), raw_string_b_string_res.c_str());
}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
