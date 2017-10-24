#include <gtest/gtest.h>

#include <Transitioner.h>
#include <TransitionFunctions.h>

namespace
{
class TransitionerTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    TransitionerTest(){}
};
TEST_F(TransitionerTest, worksWell)
{
  float testVariable = -1;
  Transitioner<float,Transition::Linear> tr(testVariable);
  tr.init(0,1,10);

  EXPECT_EQ(0,testVariable);
  tr.step(5);
  EXPECT_EQ(0.5f,testVariable);
  tr.step(2);
  EXPECT_EQ(0.7f,testVariable);
  tr.step(3);
  EXPECT_EQ(1,testVariable);
  tr.step(-5);
  EXPECT_EQ(0.5f,testVariable);
  tr.step(994564.213123f);
  EXPECT_EQ(1,testVariable);
  tr.step(-99999999);
  EXPECT_EQ(0,testVariable);
}
};