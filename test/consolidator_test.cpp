#include <gtest/gtest.h>

#include <Consolidator.h>
#include <RepeatingBuffer.h>
#include <ConsolidationMethods.h>

namespace
{
class ConsolidatorTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    DataFlow::RepeatingBuffer<float> infiniteOnes;
    DataFlow::RepeatingBuffer<float> infiniteTwos;
    ConsolidatorTest():infiniteOnes(std::vector<float>(1,1)),infiniteTwos(std::vector<float>(1,2)){}
};

TEST_F(ConsolidatorTest, attachThenDetach)
{
    DataFlow::Consolidator<float,Consolidation::Multiplication> cons;
    cons.attach(infiniteOnes);
    std::vector<float> vec;

    EXPECT_NO_THROW(vec = cons.get(512));
    for(auto e : vec){
        EXPECT_EQ(e,1);
    }

    cons.attach(infiniteTwos);
    EXPECT_NO_THROW(vec = cons.get(512));

    cons.detach(infiniteOnes);

    EXPECT_NO_THROW(vec = cons.get(512));
    for(auto e : vec){
        EXPECT_EQ(e,2);
    }
}

TEST_F(ConsolidatorTest, getSize)
{
    DataFlow::Consolidator<float,Consolidation::Multiplication> cons;
    cons.attach(infiniteOnes);
    std::vector<float> vec;
    int testSize = 512;

    EXPECT_NO_THROW(vec = cons.get(testSize));
    EXPECT_EQ(testSize,vec.size());
    
    testSize=107;
    EXPECT_NO_THROW(vec = cons.get(testSize));
    EXPECT_EQ(testSize,vec.size());
}
};