#pragma once
//!\~todo TODO: fix buffers and document!

#include <vector>

#include <iSource.h>
namespace DataFlow
{
template <typename T>
class iBuffer : public DataFlow::iSource<T>
{
  public:
    virtual bool add(T &_buffer) = 0;
    /*!
      Adds argument to buffer. Returns elements consumed.
    */
    virtual int add(std::vector<T> &_buffer) = 0;

    /*Returns whether buffer is empty*/
    virtual bool isEmpty() = 0;

    /*Returns whether buffer is full*/
    virtual bool isFull() = 0;

    /*Clears buffer of data*/
    virtual void clear() = 0;

    /*Returns capacity in number of element T*/
    virtual unsigned int capacity() = 0;

    virtual unsigned int size() = 0;

    virtual ~iBuffer(){};
};
}