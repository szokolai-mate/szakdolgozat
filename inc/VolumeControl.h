#pragma once

#include <atomic>
#include <vector>

#include <iSource.h>

template <typename T>
/*! \~english A functor that acts as volume control when used with Applicator.
    \~hungarian Egy funktor ami hangerőszabályzóként viselkedik egy Applicator -ban.
*/
class VolumeControl
{
  private:
    std::atomic<T> volume;
    DataFlow::iSource<T> *source;

  public:
    /*! \~english Get the current volume multiplier.
        \~hungarian Visszaadja a jelenlegi hangerő szorzót.
        \return \~english the volume multiplier
                \~hungarian a hangerő szorzó
    */
    T getVolume()
    {
        return volume;
    }

    /*! \~english Set the volume multiplier.
        \~hungarian Beállítja a hangerő szorzót.
        \param volume \~english the desired volume
                        \~hungarian a kívánt hangerő
    */
    void setVolume(const T &volume)
    {
        this->volume = volume;
    }

    inline T operator()(T x)
    {
       return x * volume;
    }

    inline void operator()(std::vector<T> &vector)
    {
        for (T &x : vector)
        {
            x = x * volume;
        }
    }

    VolumeControl(): volume(1){};
    VolumeControl(const float &volume) : volume(volume) {}
};