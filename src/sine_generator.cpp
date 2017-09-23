#include <SineGenerator.h>

template <typename T>
std::array<float, SINE_TABLE_SIZE> Mixer::SineGenerator<T>::sinetable;

template <typename T>
bool Mixer::SineGenerator<T>::sinetableUninitialized = true;

template <typename T>
Mixer::SineGenerator<T>::SineGenerator(const float &frequency, const unsigned int &channels, const unsigned int &sampleRate) : AudioSource(channels, sampleRate)
{
    this->step = frequency * 100;
    this->position = 0;
    if (sinetableUninitialized)
    {
        for (int i = 0; i < SINE_TABLE_SIZE; i++)
        {
            sinetable[i] = filler(i);
        }
        sinetableUninitialized = false;
    }
}

template <typename T>
std::vector<T> Mixer::SineGenerator<T>::get(const unsigned int &amount)
{
    std::vector<T> res;
    for (unsigned int count = 0; count < amount / getChannels(); count++)
    {
        for (int i = 0; i < getChannels(); i++)
        {
            res.push_back(sinetable[position]);
        }
        position = (int)round((position + step)) % SINE_TABLE_SIZE;
    }
    return res;
}

template class Mixer::SineGenerator<float>;