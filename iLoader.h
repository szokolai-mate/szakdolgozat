#pragma once

#include <string>
#include <future>

#include "iBuffer.h"
#include "utils.h"

template <typename T>
class iLoader{
    //TODO: feaure evaluation
public:
    virtual void load(iBuffer<T> & _buffer , std::promise<audio_descriptor> _promise) = 0;
    virtual void pause_load() = 0;
    virtual bool open(const std::string &) = 0;
};