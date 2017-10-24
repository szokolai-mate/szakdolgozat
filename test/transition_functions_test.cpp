#include <gtest/gtest.h>

#include <TransitionFunctions.h>

namespace
{
class TransitionFunctionsTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    TransitionFunctionsTest(){}
};

TEST_F(TransitionFunctionsTest, linear)
{
    Transition::Linear<float> tr;
    tr.init(0,0,1,1);

    EXPECT_EQ(0,tr(0));
    EXPECT_EQ(1,tr(1));
    EXPECT_EQ(0.5f,tr(0.5f));
    EXPECT_EQ(0.8f,tr(0.8f));
}

TEST_F(TransitionFunctionsTest, easeIn)
{
    Transition::EaseIn::Cubic<float> tr;
    tr.init(0,0,1,1);

    EXPECT_EQ(0,tr(0));
    EXPECT_EQ(1,tr(1));
    EXPECT_GT(0.5f,tr(0.5f));
    EXPECT_GT(0.8f,tr(0.8f));
}

TEST_F(TransitionFunctionsTest, easeOut)
{
    Transition::EaseOut::Cubic<float> tr;
    tr.init(0,0,1,1);

    EXPECT_EQ(0,tr(0));
    EXPECT_EQ(1,tr(1));
    EXPECT_LT(0.5f,tr(0.5f));
    EXPECT_LT(0.8f,tr(0.8f));
}

TEST_F(TransitionFunctionsTest, easeInOut)
{
    Transition::EaseInOut::Cubic<float> tr;
    tr.init(0,0,1,1);

    EXPECT_EQ(0,tr(0));
    EXPECT_EQ(1,tr(1));
    EXPECT_EQ(0.5f,tr(0.5f));
    EXPECT_GT(0.3f,tr(0.3f));
    EXPECT_LT(0.8f,tr(0.8f));
}
};