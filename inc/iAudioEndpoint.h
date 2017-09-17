#pragma once

#include <vector>

#include <iSource.h>

namespace Mixer{
    template <typename T>
    class iAudioEndpoint{
    public:
        virtual bool init(DataFlow::iSource<T> & source, unsigned int channels, unsigned int sampleRate) = 0;
    };
}