#pragma once

#include <portaudio.h>
#include <thread>
#include <chrono>

#include <AudioSource.h>
#include <iBuffer.h>

#define PA_WRITE_ERRORS

#ifdef PA_WRITE_ERRORS
#include <iostream>
#endif

namespace Mixer{
/*!
    \~english This class will use the PortAudio backend to record audio data from the computer's default input device into the attached buffer.
    \~hungarian Ez az osztály a PortAudio backend segítségével képes az alapértelmezett bemenetről audió adatot felvenni és azt a csatolt pufferbe besorolni.
*/
template <typename T>
class Recorder : public AudioSource{
private:
    DataFlow::iBuffer<T> * buffer;
    static int callbackFunction(const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo *timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData);
public:
    /*!
        \~english Starts recording for the given duration and streams the resulting audio data into the buffer.

        Set oneBlock to true to put the whole recorded data into the buffer in one put() .
        \~hungarian Megkezdi a felvételt a megadott időtartamig, az adatot pedig a pufferbe sorolja.

        Ha a oneBlock argumentum igaz, az egész felvétel egy put() -al kerül a pufferbe.
        \~english \param seconds the duration of the recording
        \~hungarian \param seconds a felvétel időtartama
        \~english \param oneBlock whether to use only one put() on the buffer
        \~hungarian \param oneBlock csak egy put() -ot hívhat meg a pufferre
        \~english \return whether the recording was successful
        \~hungarian \return a felvétel sikeres volt-e
    */
    bool record(const float & seconds, bool oneBlock = false);

    /*!
        \~english Attach the parameter as this object's buffer.
        \~hungarian A paramétert az objektum puffereként hozzácsatolja.
        \~english \param buffer the iBuffer<T> to attach
        \~hungarian \param buffer a csatolni kívánt iBuffer<T>
    */
    void attach(DataFlow::iBuffer<T> & buffer){
        this->buffer = &buffer;
    }

    /*! \~english Constructor. Requires parameters for AudioSource.
        \~hungarian Konstuktor. Az AudioSource paramétereit kéri.

        \~english \param channels channel count
                        \~hungarian \param channels csatornák száma
        \~english \param sampleRate sample rate
                            \~hungarian \param sampleRate mintavételezési sűrűség
    */
    Recorder(const unsigned int & channels, const unsigned int & sampleRate) : AudioSource(channels,sampleRate){}
};
};