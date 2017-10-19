#include <gtest/gtest.h>

#include <PortAudioBackend.h>
#include <RepeatingBuffer.h>

#define CHANNELS 2
#define SAMPLE_RATE 48000

namespace
{
class PortAudioBackendTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    PortAudioBackendTest(): silence(std::vector<float>(512,0)){}
    DataFlow::RepeatingBuffer<float> silence;
    Mixer::PortAudioBackend<float> backend;
};

TEST_F(PortAudioBackendTest, ErrorInInit)
{
    EXPECT_FALSE(backend.init(silence,0,0));
}

TEST_F(PortAudioBackendTest, SuccessfulInit)
{
    EXPECT_TRUE(backend.init(silence,CHANNELS,SAMPLE_RATE));
}

TEST_F(PortAudioBackendTest, CloseStream)
{
    EXPECT_TRUE(backend.init(silence,CHANNELS,SAMPLE_RATE));
    EXPECT_TRUE(backend.close());
    EXPECT_FALSE(backend.close());
}

TEST_F(PortAudioBackendTest, StartAndStop)
{
    EXPECT_TRUE(backend.init(silence,CHANNELS,SAMPLE_RATE));
    EXPECT_TRUE(backend.start());
    EXPECT_FALSE(backend.start());
    EXPECT_TRUE(backend.stop());
    EXPECT_FALSE(backend.stop());
}
};