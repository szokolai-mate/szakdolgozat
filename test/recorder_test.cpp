#include <gtest/gtest.h>

#include <Recorder.h>
#include <QueueBuffer.h>
#include <RepeatingBuffer.h>

#define CHANNELS 2
#define SAMPLE_RATE 48000

namespace
{
class RecorderTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    RecorderTest(){}
};

TEST_F(RecorderTest, PortaudioInitialized)
{
    DataFlow::QueueBuffer<float> buffer;    
    Mixer::Recorder<float> rec(CHANNELS,SAMPLE_RATE);
    rec.attach(buffer);
    EXPECT_TRUE(rec.record(0))<<"Error in PortAudio!";
}

TEST_F(RecorderTest, InvalidAudioParameters)
{
    DataFlow::QueueBuffer<float> buffer;    
    Mixer::Recorder<float> rec(0,0);
    rec.attach(buffer);
    EXPECT_FALSE(rec.record(0))<<"Error in PortAudio!";
}

TEST_F(RecorderTest, DataRecorded)
{
    DataFlow::QueueBuffer<float> buffer;    
    Mixer::Recorder<float> rec(CHANNELS,SAMPLE_RATE);
    rec.attach(buffer);
    float seconds = 0.1f;
    EXPECT_TRUE(rec.record(seconds))<<"Error in PortAudio!";
    EXPECT_GT(buffer.size(),seconds*CHANNELS*SAMPLE_RATE)<<"Recorded data is too short!";
}

TEST_F(RecorderTest, DataRecordedInOneBlock)
{
    Mixer::Recorder<float> rec(CHANNELS,SAMPLE_RATE);
    DataFlow::RepeatingBuffer<float> buffer;    
    rec.attach(buffer);
    float seconds = 0.1f;
    EXPECT_TRUE(rec.record(seconds,true))<<"Error in PortAudio!";
    EXPECT_GT(buffer.size(),seconds*CHANNELS*SAMPLE_RATE)<<"Recorded data is too short!";
}

}