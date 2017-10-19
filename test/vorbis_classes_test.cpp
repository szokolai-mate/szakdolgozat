#include <gtest/gtest.h>
#include <cstdio>

#include <OggFileLoader.h>
#include <VorbisEncoder.h>
#include <VorbisDecoder.h>
#include <RepeatingBuffer.h>
#include <QueueBuffer.h>
#include <Multiplexer.h>

#include <iostream>

#define CHANNELS 2
#define SAMPLE_RATE 48000

namespace
{
class VorbisTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    DataFlow::RepeatingBuffer<float> infiniteOnes;
    DataFlow::RepeatingBuffer<float> infiniteTwos;
    VorbisTest():infiniteOnes(std::vector<float>(1,1)),infiniteTwos(std::vector<float>(1,2)){}
};

TEST_F(VorbisTest, EncodeThenDecode)
{
  /*std::string fname{"test.ogg"};

  Mixer::Multiplexer<float> mx(2);
  mx.setChannel(1,infiniteOnes);
  mx.setChannel(2,infiniteTwos);

  VorbisEncoder encoder(CHANNELS,SAMPLE_RATE,1);
  EXPECT_TRUE(encoder.open(fname));
  encoder.addComment("test","comment");
  encoder.addComment("key","vAlUE");
  EXPECT_EQ(0,encoder.initEncoding());
  encoder.add(mx.get(SAMPLE_RATE*CHANNELS*10));
  EXPECT_TRUE(encoder.close());

  OggFileLoader<float, VorbisDecoder> loader;
	EXPECT_TRUE(loader.open(fname));
  loader.init();
  
  auto comments = loader.getDecoder().getComments();
  EXPECT_EQ(comments[0],"test=comment");
  EXPECT_EQ(comments[1],"key=vAlUE");

  DataFlow::QueueBuffer<float> buffer;
  std::vector<float> vec = loader.get(512);
  while(vec.size()>0){
    std::cout<<vec.size()<<std::endl;
    buffer.put(vec);
    vec = loader.get(512);
  }
  std::cout<<vec.size()<<std::endl;
  
  loader.close();
  std::remove(fname.c_str());*/
}
};