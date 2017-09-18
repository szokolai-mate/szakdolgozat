#pragma once
#include <portaudio.h>
#include <vector>

#include <iAudioBackend.h>
#include <iSource.h>


#define PA_WRITE_ERRORS

#ifdef PA_WRITE_ERRORS
#include <iostream>
#endif


/*TODO: at start and stop make a rapid approach to 0 to avoid popping*/

//! \~english An endpoint with PortAudio backend
//! \~hungarian PortAudio backendet használó végpont
/*!
    \~english This class utilizes the cross-platform audio backend PortAudio to play data from an iSource.
    The init() function must be called before starting playback with the start() function.
    The source must contain the audio data already multiplexed.
    \~hungarian Ez az osztály egy multiplatformos audió backendet, a PortAudio-t használja egy iSource objektumból nyert audió adat lejátszására.
    Használat előtt az init() függvénnyel egy iSource objektumot csatlakoztatunk hozzá, majd a start() fügvénnyel elkezdhetjük a lejátszást.
    A csatolt objektumból nyert adat multiplexáltnak tekintendő.
*/
template <typename T>
class PortAudioBackend : public Mixer::iAudioBackend<T>
{
  public:
    /*! Initialize the backend with the parameters.
    \param source the object supplying audio data
    \param channels channel count
    \param sampleRate sample rate
    */
    bool init(DataFlow::iSource<T> & source,unsigned int channels, unsigned int sampleRate);
    //! Start playing from the attached source.
    bool start();
    //! Stop playing. Idempotent.
    bool stop();
    //! Close stream. Reopen with init().
    bool close();

  private:
    PaStream *stream;
    PaError err;
    DataFlow::iSource<T> *source;
    static int callbackFunction(const void *inputBuffer, void *outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo *timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void *userData);
};

template <>
int PortAudioBackend<float>::callbackFunction(const void *inputBuffer, void *outputBuffer,
                                               unsigned long framesPerBuffer,
                                               const PaStreamCallbackTimeInfo *timeInfo,
                                               PaStreamCallbackFlags statusFlags,
                                               void *userData);