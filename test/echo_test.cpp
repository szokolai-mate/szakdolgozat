#include <gtest/gtest.h>

#include <Filters.h>

namespace
{
class EchoTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    EchoTest(){}
};

TEST_F(EchoTest, worksWithDelay)
{
    Mixer::Filter::Echo<float> echo;  
    echo.setReflectionQuotient(0.5f);
    echo.setDelay(1);

    float e = 99;
    e = echo(e);
    EXPECT_EQ(e,99);
    e = echo(e);
    EXPECT_GT(e,99);

}

TEST_F(EchoTest, clear)
{
    Mixer::Filter::Echo<float> echo;  
    echo.setReflectionQuotient(0.5f);
    float e = 99;
    echo(e);
    echo.clear();
    e = 0;
    echo(e);
    EXPECT_EQ(0,e);
}

TEST_F(EchoTest, getDelay)
{
    Mixer::Filter::Echo<float> echo;  
    EXPECT_EQ(0,echo.getDelay());
}

TEST_F(EchoTest, setDelay)
{
    Mixer::Filter::Echo<float> echo;
    echo.setDelay(55); 
    EXPECT_EQ(55,echo.getDelay());
}

TEST_F(EchoTest, getReflectionQuotient)
{
    Mixer::Filter::Echo<float> echo;  
    EXPECT_EQ(0,echo.getReflectionQuotient());
}

TEST_F(EchoTest, setReflectionQuotient)
{
    Mixer::Filter::Echo<float> echo;
    echo.setReflectionQuotient(0.3f); 
    EXPECT_EQ(0.3f,echo.getReflectionQuotient());
}
};