#include <gtest/gtest.h>

#include <SquareGenerator.h>

#define FREQUENCY 440
#define CHANNELS 2
#define SAMPLE_RATE 48000
#define COMPONENT_AMOUNT 50

namespace
{
class SquareGeneratorTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}

    Mixer::SquareGenerator<float> generator;
  public:
    SquareGeneratorTest():generator(FREQUENCY,COMPONENT_AMOUNT,CHANNELS,SAMPLE_RATE){}
};
TEST_F(SquareGeneratorTest, getFrequency)
{
    EXPECT_EQ(FREQUENCY,generator.getFrequency());
}

TEST_F(SquareGeneratorTest, setFrequency)
{
    float testFrequency = 55.78f;
    generator.setFrequency(testFrequency);
    EXPECT_EQ(testFrequency,generator.getFrequency());
}

TEST_F(SquareGeneratorTest, setFrequencyWithTransition)
{
    float testFrequency = 55.78f;
    float seconds = 1;
    generator.setFrequency(testFrequency,seconds);
    generator.get(CHANNELS*SAMPLE_RATE*seconds);
    EXPECT_EQ(testFrequency,generator.getFrequency());
}

TEST_F(SquareGeneratorTest, getSize)
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