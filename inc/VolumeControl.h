#pragma once

#include <atomic>
#include <vector>

#include <iSource.h>

template <typename T>
class VolumeControl : public DataFlow::iSource<T>
{
  private:
    std::atomic<T> volume = 1;
    DataFlow::iSource<T> *source;

  public:
    T getVolume()
    {
        return volume;
    }

    void setVolume(const T &volume)
    {
        this->volume = volume;
    }

    std::vector<T> get(const unsigned int &amount)
    {
        std::vector<T> res = source->get(amount);
        for (auto &e : res)
        {
            e = e * volume;
        }
        return res;
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

    VolumeControl(){};
    VolumeControl(const float &volume) : volume(volume) {}
};