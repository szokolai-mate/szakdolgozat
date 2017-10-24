#include <gtest/gtest.h>

#include <RepeatingBuffer.h>

namespace
{
class RepeatingBufferTest : public ::testing::Test
{
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
  public:
    RepeatingBufferTest(){}
};

TEST_F(RepeatingBufferTest, IsDataStoredFully)
{
    DataFlow::RepeatingBuffer<float> buffer;
    std::vector<float> data;
    for(int i = 0;i<500;i++){
        data.push_back(i);
    }
    buffer.put(data);
    EXPECT_EQ(data.size(),(buffer.get(buffer.size())).size());

    DataFlow::RepeatingBuffer<int> bufferInt;
    std::vector<int> dataInt;
    for(int i = 0;i<500;i++){
        dataInt.push_back(i);
    }
    bufferInt.put(dataInt);
    EXPECT_EQ(dataInt.size(),(bufferInt.get(bufferInt.size())).size());
}

TEST_F(RepeatingBufferTest, IsDataStoredFullyWithConstructor)
{
    std::vector<float> data;
    for(int i = 0;i<500;i++){
        data.push_back(i);
    }
    DataFlow::RepeatingBuffer<float> buffer(data);    
    EXPECT_EQ(data.size(),buffer.get(buffer.size()).size());

    std::vector<int> dataInt;
    for(int i = 0;i<500;i++){
        dataInt.push_back(i);
    }
    DataFlow::RepeatingBuffer<int> bufferInt(dataInt);    
    EXPECT_EQ(dataInt.size(),bufferInt.get(bufferInt.size()).size());
}

TEST_F(RepeatingBufferTest, getSize)
{
    std::vector<float> data;
    for(int i = 0;i<500;i++){
        data.push_back(i);
    }
    DataFlow::RepeatingBuffer<float> buffer(data);        
    int testSize = 512;
    auto vec = buffer.get(testSize);
    EXPECT_EQ(vec.size(),testSize);
    
    std::vector<int> dataInt;
    for(int i = 0;i<500;i++){
        dataInt.push_back(i);
    }
    DataFlow::RepeatingBuffer<int> bufferInt(dataInt);        
    auto vecInt = bufferInt.get(testSize);
    EXPECT_EQ(vecInt.size(),testSize);
}

TEST_F(RepeatingBufferTest, clear)
{
    std::vector<float> data;
    data.push_back(1);
    for(int i = 2;i<500;i++){
        data.push_back(i/500);
    }
    DataFlow::RepeatingBuffer<float> buffer(data);
    buffer.get(49);
    buffer.clear();
    
    EXPECT_EQ(1,buffer.get(1)[0]);

    std::vector<float> intData;
    intData.push_back(1);
    for(int i = 0;i<500;i++){
        intData.push_back(i/500);
    }
    DataFlow::RepeatingBuffer<float> intBuffer(intData);
    intBuffer.get(49);
    intBuffer.clear();
    
    EXPECT_EQ(1,intBuffer.get(1)[0]);
}
};