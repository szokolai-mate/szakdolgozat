#pragma once

#include <vector>

#include <iSource.h>

namespace Mixer{
    template <typename T>
    class iAudioBackend{
    public:
        virtual bool init(DataFlow::iSource<T> & source, const unsigned int & channels, const unsigned int & sampleRate) = 0;
        virtual ~iAudioBackend(){};
    };
};