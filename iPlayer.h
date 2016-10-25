#pragma once

#include "iBuffer.h"
#include "utils.h"

template <typename T>
class iPlayer {
public:
    //TODO: feature evaulation
    virtual bool init(iBuffer<T> & _buffer,const audio_descriptor ad) = 0;
    virtual bool pause() = 0;
    virtual bool stop() = 0;
    virtual bool play() = 0;
protected:
    T volume;
    iBuffer<T> * buffer;
};