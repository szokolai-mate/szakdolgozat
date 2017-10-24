#pragma once

#include <iPlayer.h>
#include <iSource.h>
#include <AudioSource.h>

#include <vector>
#include <atomic>

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
            return std::vector<T>(amount*getChannels(),0);
        }
        auto vec = source->get(amount * getChannels());
        if(vec.size()>0){
            return vec;
        }
        return std::vector<T>(amount*getChannels(),0);
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
        backend.start();
        return paused;
    }
    /*!
        \~english Attach the parameter as this object's source.
        \~hungarian A paramétert az objektum forrásaként hozzácsatolja.
        \~english \param source the iSource<T> to attach
                        \~hungarian \param source a csatolni kívánt iSource<T>
    */
    void attach(DataFlow::iSource<T> &source)
    {
        this->source = &source;
    }

    /*! \~english Constructor. Requires parameters for AudioSource.
        \~hungarian Konstuktor. Az AudioSource paramétereit kéri.

        \~english \param channels channel count
                        \~hungarian \param channels csatornák száma
        \~english \param sampleRate sample rate
                            \~hungarian \param sampleRate mintavételezési sűrűség
    */
    SimplePlayer(const unsigned int &channels, const unsigned int &sampleRate) : AudioSource(channels, sampleRate)
    {
        backend.init(*this, channels, sampleRate);
    }
};
};