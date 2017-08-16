#pragma once

#include <iostream>
#include <portaudio.h>
#include <mutex>
#include <condition_variable>

#include <iPlayer.h>
#include <iBuffer.h>
#include <utils.h>
//#include "sine_generator.h"

template <typename T>
class SimplePlayer : public iPlayer<T>
{
  public:
    bool play();
    bool pause();
    bool stop();
    bool init(iBuffer<T> &_buffer, const audio_descriptor ad);

  private:
    PaStream *stream;
    PaError err;
    static int bufferCallback(const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo *timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData);
    static int sineCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);
};