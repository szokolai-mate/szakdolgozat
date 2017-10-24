#include <gtest/gtest.h>

#include <Clipping.h>
#include <RepeatingBuffer.h>

namespace
{
class ClippingTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    ClippingTest(){}
};

TEST_F(ClippingTest, Hard)
{
    Clipping::Hard<float> h;
    EXPECT_EQ(0,h(0));
    EXPECT_EQ(0.7f,h(0.7f));
    EXPECT_EQ(1,h(1));
    EXPECT_EQ(1,h(1.1f));
    EXPECT_EQ(1,h(88.88f));
    EXPECT_EQ(-1,h(-1));
    EXPECT_EQ(-1,h(-1.2f));
    EXPECT_EQ(-1,h(-88.88f));
}

TEST_F(ClippingTest, Soft)
{
    Clipping::Soft<float> h;
    EXPECT_EQ(0,h(0));
    EXPECT_EQ(0.7f,h(0.7f));
    EXPECT_GT(1,h(1));
    EXPECT_EQ(1,h(50));
    EXPECT_LT(-1,h(-1));
    EXPECT_EQ(-1,h(-50));
}
};