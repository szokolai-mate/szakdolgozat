#include <gtest/gtest.h>

#include <Multiplexer.h>
#include <RepeatingBuffer.h>

namespace
{
class MultiplexerTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    DataFlow::RepeatingBuffer<float> infiniteOnes;
    DataFlow::RepeatingBuffer<float> infiniteTwos;
    MultiplexerTest():infiniteOnes(std::vector<float>(1,1)),infiniteTwos(std::vector<float>(1,2)){}
};

TEST_F(MultiplexerTest, ChannelCountFromConstructor)
{
    Mixer::Multiplexer<float> mx(4);
    EXPECT_EQ(4,mx.getChannelCount());
}

TEST_F(MultiplexerTest, DefaultChannelCount)
{
    Mixer::Multiplexer<float> mx;
    EXPECT_EQ(2,mx.getChannelCount());    
}

TEST_F(MultiplexerTest, setChannelCount)
{
    Mixer::Multiplexer<float> mx(2);
    EXPECT_EQ(2,mx.getChannelCount());
    mx.setChannelCount(1);
    EXPECT_EQ(1,mx.getChannelCount());    
}

TEST_F(MultiplexerTest, getSize)
{
    int testSize = 512;
    int channels = 3;
    Mixer::Multiplexer<float> mx(channels);

    auto vec = mx.get(testSize*channels);
    EXPECT_EQ(vec.size(),testSize*channels);
    testSize = 499;
    vec = mx.get(testSize*channels);
    EXPECT_EQ(vec.size(),testSize*channels);
    testSize = 107;
    vec = mx.get(testSize*channels);
    EXPECT_EQ(vec.size(),testSize*channels);
}

TEST_F(MultiplexerTest, clearChannel)
{
  Mixer::Multiplexer<float> mx(1);
  auto vec = mx.get(512);
  for(auto e : vec){
    EXPECT_EQ(0,e);    
  }
  mx.setChannel(1,infiniteOnes);
  vec = mx.get(512);
  for(auto e : vec){
    EXPECT_EQ(1,e);    
  }
  mx.clearChannel(1);
  vec = mx.get(512);
  for(auto e : vec){
    EXPECT_EQ(0,e);    
  }
}

TEST_F(MultiplexerTest, clear)
{
  int channels = 3;
  Mixer::Multiplexer<float> mx(channels);
  auto vec = mx.get(512);
  for(auto e : vec){
    EXPECT_EQ(0,e);    
  }
  mx.setChannel(2,infiniteOnes);
  mx.setChannel(3,infiniteTwos);
  int which = 0;
  for(auto e : mx.get(512*channels)){
    EXPECT_EQ(e,which);
    which=(which+1)%channels;
  }
  mx.clear();
  vec = mx.get(512);
  for(auto e : vec){
    EXPECT_EQ(0,e);    
  }
}

TEST_F(MultiplexerTest, SuppliedDataIsCorrect)
{
  int channels = 3;
  Mixer::Multiplexer<float> mx(channels);
  mx.setChannel(2,infiniteOnes);
  mx.setChannel(3,infiniteTwos);
  int which = 0;
  for(auto e : mx.get(512*channels)){
    EXPECT_EQ(e,which);
    which=(which+1)%channels;
  }
}
};