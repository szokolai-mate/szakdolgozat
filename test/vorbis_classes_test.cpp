#include <gtest/gtest.h>
#include <cstdio>

#include <OggFileLoader.h>
#include <VorbisEncoder.h>
#include <VorbisDecoder.h>
#include <RepeatingBuffer.h>
#include <Multiplexer.h>

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
  Mixer::Multiplexer<float> mx(2);
  mx.setChannel(1,infiniteOnes);
  mx.setChannel(2,infiniteTwos);
  /*{
    VorbisEncoder encoder(CHANNELS,SAMPLE_RATE,1);
    encoder.open("test.ogg");
    encoder.addComment("test","comment");
    encoder.addComment("key","value");
    encoder.initEncoding();
    encoder.add(mx.get(512));
    encoder.close();
  }*/

  std::remove("test.ogg");
}
};