#include <gtest/gtest.h>

#include <Consolidator.h>
#include <ConsolidationMethods.h>
#include <RepeatingBuffer.h>

namespace
{
class ConsolidationMethodTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    ConsolidationMethodTest(){}
};

TEST_F(ConsolidationMethodTest, AccumulationWithFloat)
{
    DataFlow::Consolidator<float,Consolidation::Accumulation> cons;

    DataFlow::RepeatingBuffer<float> sourceOne(std::vector<float>{1.0f,3,7});
    DataFlow::RepeatingBuffer<float> sourceTwo(std::vector<float>{0,1,0.5f});
    DataFlow::RepeatingBuffer<float> sourceThree(std::vector<float>{2,3.2f,4});
    DataFlow::RepeatingBuffer<float> expectedSource(std::vector<float>{3,7.2f,11.5f});

    cons.attach(sourceOne);
    cons.attach(sourceTwo);
    cons.attach(sourceThree);

    int testSize = 512;
    auto res = cons.get(testSize);
    auto exp = expectedSource.get(testSize);
    for(int i = 0 ;i<testSize;i++){
        EXPECT_EQ(res[i],exp[i]);
    }
}

TEST_F(ConsolidationMethodTest, AccumulationWithint)
{
    DataFlow::Consolidator<int,Consolidation::Accumulation> cons;

    DataFlow::RepeatingBuffer<int> sourceOne(std::vector<int>{1,3,7});
    DataFlow::RepeatingBuffer<int> sourceTwo(std::vector<int>{0,1,0});
    DataFlow::RepeatingBuffer<int> sourceThree(std::vector<int>{2,-3,4});
    DataFlow::RepeatingBuffer<int> expectedSource(std::vector<int>{3,1,11});

    cons.attach(sourceOne);
    cons.attach(sourceTwo);
    cons.attach(sourceThree);

    int testSize = 512;
    auto res = cons.get(testSize);
    auto exp = expectedSource.get(testSize);
    for(int i = 0 ;i<testSize;i++){
        EXPECT_EQ(res[i],exp[i]);
    }
}

TEST_F(ConsolidationMethodTest, MultiplicationWithFloat)
{
    DataFlow::Consolidator<float,Consolidation::Multiplication> cons;

    DataFlow::RepeatingBuffer<float> sourceOne(std::vector<float>{1.0f,3,7});
    DataFlow::RepeatingBuffer<float> sourceTwo(std::vector<float>{0,1,0.5f});
    DataFlow::RepeatingBuffer<float> sourceThree(std::vector<float>{2,3.2f,4});
    DataFlow::RepeatingBuffer<float> expectedSource(std::vector<float>{0,9.6f,14});

    cons.attach(sourceOne);
    cons.attach(sourceTwo);
    cons.attach(sourceThree);

    int testSize = 512;
    auto res = cons.get(testSize);
    auto exp = expectedSource.get(testSize);
    for(int i = 0 ;i<testSize;i++){
        EXPECT_EQ(res[i],exp[i]);
    }
}

TEST_F(ConsolidationMethodTest, MultiplicationWithInt)
{
    DataFlow::Consolidator<int,Consolidation::Multiplication> cons;

    DataFlow::RepeatingBuffer<int> sourceOne(std::vector<int>{1,3,7});
    DataFlow::RepeatingBuffer<int> sourceTwo(std::vector<int>{0,1,0});
    DataFlow::RepeatingBuffer<int> sourceThree(std::vector<int>{2,-3,4});
    DataFlow::RepeatingBuffer<int> expectedSource(std::vector<int>{0,-9,0});

    cons.attach(sourceOne);
    cons.attach(sourceTwo);
    cons.attach(sourceThree);

    int testSize = 512;
    auto res = cons.get(testSize);
    auto exp = expectedSource.get(testSize);
    for(int i = 0 ;i<testSize;i++){
        EXPECT_EQ(res[i],exp[i]);
    }
}
};