#pragma once

#include <iPlayer.h>
#include <iSource.h>
#include <AudioSource.h>

#include <vector>
#include <atomic>

/*TODO: at start and stop make a rapid approach to 0 to avoid popping*/

template <typename T, template <typename> typename K>
class SimplePlayer : public Mixer::AudioSource, public DataFlow::iSource<T>, public iPlayer
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
            //TODO
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

    SimplePlayer(const unsigned int &channels, const unsigned int &sampleRate) : AudioSource(channels, sampleRate)
    {
        backend.init(*this, channels, sampleRate);
    }
};