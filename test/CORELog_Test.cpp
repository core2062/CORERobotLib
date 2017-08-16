#include "gtest/gtest.h"
//
//#include "CORELogging/CORELog.h"

TEST(FancyTest, Test1) {
    ASSERT_EQ(1, 1);
}

TEST(FancyTest, Test2) {
    ASSERT_EQ(-1, -1);
}

TEST(Good, GoodGTest) {

}

//TEST(CORELog, CORELogInit) {
//    CORE::CORELog::robotInit();
//    ASSERT_NO_THROW(CORE::CORELog::getName());
//}