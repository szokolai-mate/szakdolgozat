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
        if (transitioner)
        {
            transitioner->step(1);
        }
    }
    return res;
}

template <typename T>
float Mixer::SineGenerator<T>::getFrequency() const
{
    return step / (SINE_TABLE_SIZE / (float)getSampleRate());
}

template <typename T>
void Mixer::SineGenerator<T>::setFrequency(const float &frequency)
{
    this->step = frequency * (SINE_TABLE_SIZE / (float)getSampleRate());
}

template <typename T>
void Mixer::SineGenerator<T>::setFrequency(const float &frequency, const float &seconds, std::string method, const unsigned int &degree)
{
    float newStep = frequency * (SINE_TABLE_SIZE / (float)getSampleRate());
    float samples = seconds * getSampleRate();

    if ("linear" == method)
    {
        this->transitioner = std::make_unique<Transitioner<T, Transition::Linear>>(this->step);
    }
    switch (degree)
    {
    case 2:
        if ("ease-in-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Quadratic>>(this->step);
            break;
        }
        if ("ease-in" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Quadratic>>(this->step);
            break;
        }
        if ("ease-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Quadratic>>(this->step);
            break;
        }

    case 3:
        if ("ease-in-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Cubic>>(this->step);
            break;
        }
        if ("ease-in" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Cubic>>(this->step);
            break;
        }
        if ("ease-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Cubic>>(this->step);
            break;
        }

    case 4:
        if ("ease-in-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Quartic>>(this->step);
            break;
        }
        if ("ease-in" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Quartic>>(this->step);
            break;
        }
        if ("ease-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Quartic>>(this->step);
            break;
        }

    case 5:
            if ("ease-in-out" == method)
            {
                this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Quintic>>(this->step);
                break;
            }
            if ("ease-in" == method)
            {
                this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Quintic>>(this->step);
                break;
            }
            if ("ease-out" == method)
            {
                this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Quintic>>(this->step);
                break;
            }
    default:
        this->transitioner = std::make_unique<Transitioner<T, Transition::Linear>>(this->step);
    }

    this->transitioner->init(step, newStep, samples);
}

template class Mixer::SineGenerator<float>;