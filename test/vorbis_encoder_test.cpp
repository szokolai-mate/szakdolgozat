#include <gtest/gtest.h>

#include <VorbisEncoder.h>

#define CHANNELS 2
#define SAMPLE_RATE 48000

namespace
{
class VorbisEncoderTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    VorbisEncoderTest(){}
};

TEST_F(VorbisEncoderTest, QualityOutOfRange)
{
    EXPECT_NO_THROW(new VorbisEncoder(CHANNELS,SAMPLE_RATE,-99));
    EXPECT_NO_THROW(new VorbisEncoder(CHANNELS,SAMPLE_RATE,2312376));
}
};