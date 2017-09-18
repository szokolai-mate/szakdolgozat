#pragma once

#include <atomic>

#include <iSource.h>

template <typename T>
class iPlayer : public DataFlow::iSource<T> {
public:
    //TODO: feature evaulation
    virtual bool init(DataFlow::iSource, unsigned int channels, unsigned int sampleRate) = 0;
    virtual bool pause() = 0;
    virtual bool stop() = 0;
    virtual bool play() = 0;
protected:
    T volume;
    std::atomic<bool> paused;
};