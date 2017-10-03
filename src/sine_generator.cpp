#include <SineGenerator.h>

template <typename T>
std::array<float, SINE_TABLE_SIZE> Mixer::SineGenerator<T>::sinetable;

template <typename T>
bool Mixer::SineGenerator<T>::sinetableUninitialized = true;

template <typename T>
Mixer::SineGenerator<T>::SineGenerator(const float &frequency, const unsigned int &channels, const unsigned int &sampleRate) : AudioSource(channels, sampleRate)
{
    this->step = frequency * (SINE_TABLE_SIZE / (float)sampleRate);
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
        if(transitioner){
            transitioner->step(1);
        }
    }
    return res;
}

template <typename T>
float Mixer::SineGenerator<T>::getFrequency() const{
    return step / (SINE_TABLE_SIZE / (float)getSampleRate());
}

template <typename T>
void Mixer::SineGenerator<T>::setFrequency(const float & frequency){
    this->step = frequency * (SINE_TABLE_SIZE / (float)getSampleRate());
}

template <typename T>
void Mixer::SineGenerator<T>::setFrequency(const float & frequency, const float & seconds){
    float newStep = frequency * (SINE_TABLE_SIZE / (float)getSampleRate());
    float samples = seconds * getSampleRate();
    if(transitioner) delete transitioner;
    this->transitioner = new Transitioner<T,Transition::EaseInOut::Quintic>(this->step);
    this->transitioner->init(step,newStep,samples);
}


template class Mixer::SineGenerator<float>;