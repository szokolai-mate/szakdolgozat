#include <gtest/gtest.h>
#include <cstdio>
#include <cmath>

#include <OggFileLoader.h>
#include <VorbisEncoder.h>
#include <VorbisDecoder.h>
#include <RepeatingBuffer.h>
#include <QueueBuffer.h>
#include <Multiplexer.h>

#define CHANNELS 2
#define SAMPLE_RATE 48000

namespace
{
class CombinedVorbisTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    DataFlow::RepeatingBuffer<float> infiniteOnes;
    DataFlow::RepeatingBuffer<float> infiniteTwos;
    CombinedVorbisTest():infiniteOnes(std::vector<float>(1,1)),infiniteTwos(std::vector<float>(1,2)){}
};

TEST_F(CombinedVorbisTest, EncodeThenDecode)
{
  std::string fname{"test.ogg"};

  float seconds = 10;

  Mixer::Multiplexer<float> mx(2);
  mx.setChannel(1,infiniteOnes);
  mx.setChannel(2,infiniteTwos);

  VorbisEncoder encoder(CHANNELS,SAMPLE_RATE,1);
  EXPECT_TRUE(encoder.open(fname));
  encoder.addComment("test","comment");
  encoder.addComment("key","vAlUE");
  EXPECT_EQ(0,encoder.initEncoding());
  encoder.add(mx.get(SAMPLE_RATE*CHANNELS*seconds));
  EXPECT_TRUE(encoder.close());

  OggFileLoader<float, VorbisDecoder> loader;
	EXPECT_TRUE(loader.open(fname));
  loader.init();

  /*Header is correct*/
  EXPECT_EQ(CHANNELS,loader.getDecoder().getChannels());
  EXPECT_EQ(SAMPLE_RATE,loader.getDecoder().getSampleRate());
  
  /*Comments are correct*/
  auto comments = loader.getDecoder().getComments();
  EXPECT_EQ(comments[0],"test=comment");
  EXPECT_EQ(comments[1],"key=vAlUE");

  DataFlow::QueueBuffer<float> buffer;
  std::vector<float> vec = loader.get(512);
  while(vec.size()>0){
    buffer.put(vec);
    vec = loader.get(512);
  }

  /*Vendor string is not empty*/
  EXPECT_GT((loader.getDecoder().getVendor()).size(),0);
  
  
  /*All data present*/
  EXPECT_EQ(buffer.size(),CHANNELS*SAMPLE_RATE*seconds);

  /*Values are within error range and are in order*/
  float error = 0.1f;
  int nextValue = 1;
  for(auto e : buffer.get(buffer.size())){
    EXPECT_GT(nextValue,fabs(e-error));
    if(nextValue==1){
      nextValue = 2;
    }
    else{
      nextValue = 1;
    }
  }

  loader.close();
  std::remove(fname.c_str());
}
};