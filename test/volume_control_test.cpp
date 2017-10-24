#include <gtest/gtest.h>

#include <VolumeControl.h>
#include <RepeatingBuffer.h>

namespace
{
class VolumeControlTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}

    VolumeControl<float> vc;
  public:
    DataFlow::RepeatingBuffer<float> infiniteOnes;
    DataFlow::RepeatingBuffer<float> infiniteTwos;
    VolumeControlTest():infiniteOnes(std::vector<float>(1,1)),infiniteTwos(std::vector<float>(1,2)){}
};
TEST_F(VolumeControlTest, getVolume)
{
    EXPECT_EQ(1,vc.getVolume());
}

TEST_F(VolumeControlTest, setVolume)
{
    vc.setVolume(0.1f);
    EXPECT_EQ(0.1f,vc.getVolume());    
}

TEST_F(VolumeControlTest, setVolumeWithTransition)
{
    float seconds = 1;
    int valuesPerSecond = 60;
    float testVolume = 0.1f;
    vc.setVolume(testVolume, seconds, valuesPerSecond);
    EXPECT_EQ(1,vc.getVolume());
    for(int i = 0; i<valuesPerSecond*seconds;i++){vc(i);}
    EXPECT_EQ(testVolume,vc.getVolume());
}

TEST_F(VolumeControlTest, differentTransitionMethods)
{
    float seconds = 1;
    int valuesPerSecond = 60;
    
    float testVolume = 0.1f;

    vc.setVolume(testVolume, seconds, valuesPerSecond,"ease-in",2);
    for(int i = 0; i<valuesPerSecond*seconds;i++){vc(i);}
    EXPECT_EQ(testVolume,vc.getVolume());
    testVolume-=1;
    vc.setVolume(testVolume, seconds, valuesPerSecond,"ease-in",3);
    for(int i = 0; i<valuesPerSecond*seconds;i++){vc(i);}
    EXPECT_EQ(testVolume,vc.getVolume());
    testVolume-=1;
    vc.setVolume(testVolume, seconds, valuesPerSecond,"ease-in",4);
    for(int i = 0; i<valuesPerSecond*seconds;i++){vc(i);}
    EXPECT_EQ(testVolume,vc.getVolume());
    testVolume-=1;
    vc.setVolume(testVolume, seconds, valuesPerSecond,"ease-in",5);
    for(int i = 0; i<valuesPerSecond*seconds;i++){vc(i);}
    EXPECT_EQ(testVolume,vc.getVolume());
    testVolume-=1;

    vc.setVolume(testVolume, seconds, valuesPerSecond,"ease-out",2);
    for(int i = 0; i<valuesPerSecond*seconds;i++){vc(i);}
    EXPECT_EQ(testVolume,vc.getVolume());
    testVolume-=1;
    vc.setVolume(testVolume, seconds, valuesPerSecond,"ease-out",3);
    for(int i = 0; i<valuesPerSecond*seconds;i++){vc(i);}
    EXPECT_EQ(testVolume,vc.getVolume());
    testVolume-=1;
    vc.setVolume(testVolume, seconds, valuesPerSecond,"ease-out",4);
    for(int i = 0; i<valuesPerSecond*seconds;i++){vc(i);}
    EXPECT_EQ(testVolume,vc.getVolume());
    testVolume-=1;
    vc.setVolume(testVolume, seconds, valuesPerSecond,"ease-out",5);
    for(int i = 0; i<valuesPerSecond*seconds;i++){vc(i);}
    EXPECT_EQ(testVolume,vc.getVolume());
    testVolume-=1;
 
    vc.setVolume(testVolume, seconds, valuesPerSecond,"ease-in-out",2);
    for(int i = 0; i<valuesPerSecond*seconds;i++){vc(i);}
    EXPECT_EQ(testVolume,vc.getVolume());
    testVolume-=1;
    vc.setVolume(testVolume, seconds, valuesPerSecond,"ease-in-out",3);
    for(int i = 0; i<valuesPerSecond*seconds;i++){vc(i);}
    EXPECT_EQ(testVolume,vc.getVolume());
    testVolume-=1;
    vc.setVolume(testVolume, seconds, valuesPerSecond,"ease-in-out",4);
    for(int i = 0; i<valuesPerSecond*seconds;i++){vc(i);}
    EXPECT_EQ(testVolume,vc.getVolume());
    testVolume-=1;
    vc.setVolume(testVolume, seconds, valuesPerSecond,"ease-in-out",5);
    for(int i = 0; i<valuesPerSecond*seconds;i++){vc(i);}
    EXPECT_EQ(testVolume,vc.getVolume());
}
};