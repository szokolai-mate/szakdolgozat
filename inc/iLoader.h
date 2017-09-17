#pragma once

#include <string>
#include <future>

#include <iBuffer.h>
#include <iSource.h>
#include <Utils.h>

template <typename T>
class iLoader : public DataFlow::iSource<T>
{
    //TODO: feaure evaluation
  public:
    virtual void load(DataFlow::iBuffer<T> &_buffer, std::promise<audio_descriptor> _promise) = 0;
    virtual void pauseLoad() = 0;
    virtual bool open(const std::string &) = 0;
};