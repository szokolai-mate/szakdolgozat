#include <gtest/gtest.h>

#include <TriangleGenerator.h>

#define FREQUENCY 440
#define CHANNELS 2
#define SAMPLE_RATE 48000
#define COMPONENT_AMOUNT 50

template class Mixer::TriangleGenerator<float>;

namespace
{    
class TriangleGeneratorTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}

    Mixer::TriangleGenerator<float> generator;
  public:
    TriangleGeneratorTest():generator(FREQUENCY,COMPONENT_AMOUNT,CHANNELS,SAMPLE_RATE){}
};
TEST_F(TriangleGeneratorTest, getFrequency)
{
    EXPECT_EQ(FREQUENCY,generator.getFrequency());
}

TEST_F(TriangleGeneratorTest, setFrequency)
{
    float testFrequency = 55.78f;
    generator.setFrequency(testFrequency);
    EXPECT_EQ(testFrequency,generator.getFrequency());
}

TEST_F(TriangleGeneratorTest, setFrequencyWithTransition)
{
    float testFrequency = 55.78f;
    float seconds = 1;
    generator.setFrequency(testFrequency,seconds);
    generator.get(CHANNELS*SAMPLE_RATE*seconds);
    EXPECT_EQ(testFrequency,generator.getFrequency());
}

TEST_F(TriangleGeneratorTest, getSize)
{
    int testSize = 512;
    auto vec = generator.get(testSize*CHANNELS);
    EXPECT_EQ(vec.size(),testSize*CHANNELS);
    testSize = 499;
    vec = generator.get(testSize*CHANNELS);
    EXPECT_EQ(vec.size(),testSize*CHANNELS);
    testSize = 107;
    vec = generator.get(testSize*CHANNELS);
    EXPECT_EQ(vec.size(),testSize*CHANNELS);
}
};