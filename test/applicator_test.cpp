#include <gtest/gtest.h>

#include <Applicator.h>
#include <RepeatingBuffer.h>

#define TEST_VALUE 858

template <typename T>
class ApplicatorTestFunctor{
public:
  T operator()(const T & x){
    return TEST_VALUE;
  }
  void operator()(std::vector<T> & vec){
    for(T & x : vec){
      x = TEST_VALUE;
    }
  }
};

namespace
{
class ApplicatorTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  
  public:
    DataFlow::RepeatingBuffer<float> infiniteOnes;
    DataFlow::RepeatingBuffer<float> infiniteTwos;
    ApplicatorTest():infiniteOnes(std::vector<float>(1,1)),infiniteTwos(std::vector<float>(1,2)){}
};

TEST_F(ApplicatorTest, attach)
{
  DataFlow::Applicator<float,ApplicatorTestFunctor> ap;
  ap.attach(infiniteOnes);
  EXPECT_NO_THROW(ap.get(512));
}

TEST_F(ApplicatorTest, getSize)
{
  DataFlow::Applicator<float,ApplicatorTestFunctor> ap;
  ap.attach(infiniteOnes);
  std::vector<float> vec;

  int testSize = 512;
  EXPECT_NO_THROW(vec=ap.get(testSize));
  EXPECT_EQ(testSize,vec.size());

  testSize = 107;
  EXPECT_NO_THROW(vec=ap.get(testSize));
  EXPECT_EQ(testSize,vec.size());
}

TEST_F(ApplicatorTest, usesFunctor)
{
  DataFlow::Applicator<float,ApplicatorTestFunctor> ap;
  ap.attach(infiniteOnes);
  std::vector<float> vec;
  EXPECT_NO_THROW(vec = ap.get(512));
  for(auto e : vec){
    EXPECT_EQ(TEST_VALUE,e);
  }
}
};