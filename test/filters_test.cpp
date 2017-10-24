#include <gtest/gtest.h>

#include <Filters.h>

namespace
{
class FiltersTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    FiltersTest(){}
};

TEST_F(FiltersTest, FuzzboxDegreeOfZero)
{
    Mixer::Filter::Fuzzbox<float> box;
    box.setDegree(0);

    std::vector<float> vec(500,0.5f);
    box(vec);

    for(auto e : vec){
      EXPECT_EQ(0.5f,e);
    }
}

TEST_F(FiltersTest, FuzzboxBigDegree)
{
    Mixer::Filter::Fuzzbox<float> box;
    box.setDegree(10);

    std::vector<float> vec(500,0.5f);
    box(vec);

    for(auto e : vec){
      EXPECT_EQ(1,e);
    }

    std::vector<float> vec2(500,-0.5f);
    box(vec2);

    for(auto e : vec2){
      EXPECT_EQ(-1,e);
    }
}

TEST_F(FiltersTest, getDegree)
{
    Mixer::Filter::Fuzzbox<float> box;  
    EXPECT_EQ(4,box.getDegree());
}

TEST_F(FiltersTest, setDegree)
{
    Mixer::Filter::Fuzzbox<float> box;
    box.setDegree(55); 
    EXPECT_EQ(55,box.getDegree());
}
};