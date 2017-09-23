#pragma once

namespace Mixer{
    class AudioSource{
    private:
        unsigned int channels = 2;
        unsigned int sampleRate = 48000;
    public:
        AudioSource(const unsigned int & channels, const unsigned int & sampleRate) : channels(channels) , sampleRate(sampleRate){}
        unsigned int getChannels(){return channels;}
        unsigned int getSampleRate(){return sampleRate;}
    };
};