#pragma once

#include <iPlayer.h>
#include <iSource.h>
#include <AudioSource.h>

#include <vector>
#include <atomic>

/*! \todo TODO: at start and stop make a rapid approach to 0 to avoid popping*/

namespace Mixer{
//! \~english Simple implementation of an iPlayer.
//! \~hungarian Egy egyszerű iPlayer implementáció.
template <typename T, template <typename> typename K>
class SimplePlayer : public Mixer::AudioSource, public DataFlow::iSource<T>, public Mixer::iPlayer
{
  private:
    K<T> backend;
    DataFlow::iSource<T> *source;
    std::atomic<bool> paused;

  public:
    std::vector<T> get(const unsigned int &amount)
    {
        if (paused)
        {
            //!\todo TODO: pause function
        }
        return source->get(amount * getChannels());
    }
    bool pause()
    {
        paused = true;
        return paused;
    }
    bool stop()
    {
        return backend.stop();
    }
    bool play()
    {
        this->paused = false;
        return backend.start();
    }
    /*!
        \~english Attach the parameter as this object's source.
        \~hungarian A paramétert az objektum forrásaként hozzácsatolja.
        \param source \~english the iSource<T> to attach
                        \~hungarian a csatolni kívánt iSource<T>
    */
    void attach(DataFlow::iSource<T> &source)
    {
        this->source = &source;
    }

    /*! \~english Constructor. Requires parameters for AudioSource.
        \~hungarian Konstuktor. Az AudioSource paramétereit kéri.

        \param channels \~english channel count
                        \~hungarian csatornák száma
        \param sampleRate \~english sample rate
                            \~hungarian mintavételezési sűrűség
    */
    SimplePlayer(const unsigned int &channels, const unsigned int &sampleRate) : AudioSource(channels, sampleRate)
    {
        backend.init(*this, channels, sampleRate);
    }
};
};