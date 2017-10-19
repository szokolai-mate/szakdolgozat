#include <gtest/gtest.h>

#include <ComplexWaveformGenerator.h>

#define CHANNELS 2
#define SAMPLE_RATE 48000

namespace
{
class ComplexWaveformGeneratorTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}

    Mixer::ComplexWaveformGenerator<float> generator;
  public:
    ComplexWaveformGeneratorTest():generator(std::vector<std::pair<float,float>>(),CHANNELS,SAMPLE_RATE){}
};
TEST_F(ComplexWaveformGeneratorTest, SilentWithNoComponents)
{
    auto vec = generator.get(512);
    for(auto e : vec){
        EXPECT_EQ(0,e);
    }
}

TEST_F(ComplexWaveformGeneratorTest, SetAndGetComponent)
{
    float frequency = 440.0f;
    float volume = 0.168f;
    generator.setComponent(frequency,volume);
    EXPECT_EQ(volume,generator.getComponent(frequency)); 
}

TEST_F(ComplexWaveformGeneratorTest, SetExistingComponent)
{
    float frequency = 440.0f;
    float volume = 0.168f;
    generator.setComponent(frequency,1);
    EXPECT_EQ(1,generator.getComponent(frequency));
    generator.setComponent(frequency,volume);    
    EXPECT_EQ(volume,generator.getComponent(frequency)); 
}

TEST_F(ComplexWaveformGeneratorTest, SetAndGetComponentWithTransition)
{
    float frequency = 440.0f;
    float volume = 0.168f;
    float seconds = 1;
    generator.setComponent(frequency,1);
    generator.setComponent(frequency,volume,seconds);
    //Minimal difference becouse of the equation. Need to get one more frame to truly reach equal. Does not result in audible error.
    generator.get(seconds*CHANNELS*SAMPLE_RATE+2);
    EXPECT_EQ(volume,generator.getComponent(frequency)); 
}

TEST_F(ComplexWaveformGeneratorTest, NotPresentComponentIsSilent)
{
    EXPECT_EQ(0,generator.getComponent(99999));     
}

TEST_F(ComplexWaveformGeneratorTest, getSize)
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

TEST_F(ComplexWaveformGeneratorTest, SetComponentsWithConstructor)
{
    std::vector<std::pair<float,float>> components;
    float fundamentalFrequency = 440.0f;
    components.push_back(std::make_pair(fundamentalFrequency,1));
    components.push_back(std::make_pair(fundamentalFrequency*2,0.5f));

    Mixer::ComplexWaveformGenerator<float> newGenerator(components,CHANNELS,SAMPLE_RATE);    

    EXPECT_EQ(1,newGenerator.getComponent(fundamentalFrequency)); 
    EXPECT_EQ(0.5f,newGenerator.getComponent(fundamentalFrequency*2));
    EXPECT_EQ(0,newGenerator.getComponent(fundamentalFrequency*3));    
}
};