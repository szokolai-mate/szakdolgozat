#include <gtest/gtest.h>

#include <QueueBuffer.h>

namespace
{
class QueueBufferTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    QueueBufferTest(){}
};
TEST_F(QueueBufferTest, DataRemainsContinous)
{
    unsigned int size = 50000;
    unsigned int count = 0;
    DataFlow::QueueBuffer<float> buffer;
    std::vector<float> vector1;
    std::vector<float> vector2;
    std::vector<float> vector3;
    std::vector<float> vector4;
    std::vector<float> vector5;
    
    for(int i = 0;i<10000;i++){
        vector1.push_back(count);
        count++;
    }
    for(int i = 0;i<10000;i++){
        vector2.push_back(count);
        count++;
    }
    for(int i = 0;i<10000;i++){
        vector3.push_back(count);
        count++;
    }
    for(int i = 0;i<10000;i++){
        vector4.push_back(count);
        count++;
    }
    for(int i = 0;i<10000;i++){
        vector5.push_back(count);
        count++;
    }

    buffer.put(vector1);
    buffer.put(vector2);
    buffer.put(vector3);
    buffer.put(vector4);
    buffer.put(vector5);

    int prev = -1;
    for(auto e : buffer.get(buffer.size())){
        EXPECT_EQ(prev+1,e);
        prev++;
    }
}

TEST_F(QueueBufferTest, ClearTest)
{
    DataFlow::QueueBuffer<float> buffer;
    buffer.put(std::vector<float>(50,0));

    EXPECT_GT(buffer.size(),0);

    buffer.clear();

    EXPECT_EQ(0,buffer.size())<<"QueueBuffer did not empty itself!";
}

};