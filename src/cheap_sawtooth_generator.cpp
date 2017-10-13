#include <CheapSawtoothGenerator.h>

template <typename T>
Mixer::CheapSawtoothGenerator<T>::CheapSawtoothGenerator(const float &frequency, const unsigned int &channels, const unsigned int &sampleRate) : AudioSource(channels, sampleRate) , frequency(frequency)
{
    period = getSampleRate() / frequency ;
    m = 0.5f/period;
}

template <typename T>
std::vector<T> Mixer::CheapSawtoothGenerator<T>::get(const unsigned int &amount)
{
   std::vector<T> res(amount,0);
   for(int i = 0;i<amount;i+=getChannels()){
        for(int j = 0;j<getChannels();j++)
        {
        res[i+j]=m*(position);
        }
        position+=1;
        if(position>period/2){
            position=-period/2;
        }
        if (transitioner && transitioner2)
        {
            transitioner->step(1);
            transitioner2->step(1);
        }
   }

   return res;
}

template <typename T>
float Mixer::CheapSawtoothGenerator<T>::getFrequency() const
{
    return frequency;
}

template <typename T>
void Mixer::CheapSawtoothGenerator<T>::setFrequency(const float &frequency)
{
    this->frequency = frequency;
    period = getSampleRate() / frequency ;
    m = 1.0f/period;    
}

template <typename T>
void Mixer::CheapSawtoothGenerator<T>::setFrequency(const float &frequency, const float &seconds, std::string method, const unsigned int &degree)
{
    this->frequency = frequency;
    float newPeriod = getSampleRate() / frequency ;
    float newM = 0.5f/newPeriod;
    float samples = seconds * getSampleRate();

    if ("linear" == method)
    {
        this->transitioner = std::make_unique<Transitioner<T, Transition::Linear>>(this->period);
        this->transitioner2 = std::make_unique<Transitioner<T, Transition::Linear>>(this->m);
    }
    switch (degree)
    {
    case 2:
        if ("ease-in-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Quadratic>>(this->period);
            this->transitioner2 = std::make_unique<Transitioner<T, Transition::EaseInOut::Quadratic>>(this->m);
            break;
        }
        if ("ease-in" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Quadratic>>(this->period);
            this->transitioner2 = std::make_unique<Transitioner<T, Transition::EaseIn::Quadratic>>(this->m);
            break;
        }
        if ("ease-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Quadratic>>(this->period);
            this->transitioner2 = std::make_unique<Transitioner<T, Transition::EaseOut::Quadratic>>(this->m);
            break;
        }

    case 3:
        if ("ease-in-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Cubic>>(this->period);
            this->transitioner2 = std::make_unique<Transitioner<T, Transition::EaseInOut::Cubic>>(this->m);
            break;
        }
        if ("ease-in" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Cubic>>(this->period);
            this->transitioner2 = std::make_unique<Transitioner<T, Transition::EaseIn::Cubic>>(this->m);
            break;
        }
        if ("ease-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Cubic>>(this->period);
            this->transitioner2 = std::make_unique<Transitioner<T, Transition::EaseOut::Cubic>>(this->m);
            break;
        }

    case 4:
        if ("ease-in-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Quartic>>(this->period);
            this->transitioner2 = std::make_unique<Transitioner<T, Transition::EaseInOut::Quartic>>(this->m);
            break;
        }
        if ("ease-in" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Quartic>>(this->period);
            this->transitioner2 = std::make_unique<Transitioner<T, Transition::EaseIn::Quartic>>(this->m);
            break;
        }
        if ("ease-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Quartic>>(this->period);
            this->transitioner2 = std::make_unique<Transitioner<T, Transition::EaseOut::Quartic>>(this->m);
            break;
        }

    case 5:
            if ("ease-in-out" == method)
            {
                this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Quintic>>(this->period);
                this->transitioner2 = std::make_unique<Transitioner<T, Transition::EaseInOut::Quintic>>(this->m);
                break;
            }
            if ("ease-in" == method)
            {
                this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Quintic>>(this->period);
                this->transitioner2 = std::make_unique<Transitioner<T, Transition::EaseIn::Quintic>>(this->m);
                break;
            }
            if ("ease-out" == method)
            {
                this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Quintic>>(this->period);
                this->transitioner2 = std::make_unique<Transitioner<T, Transition::EaseOut::Quintic>>(this->m);
                break;
            }
    default:
    this->transitioner = std::make_unique<Transitioner<T, Transition::Linear>>(this->period);
    this->transitioner2 = std::make_unique<Transitioner<T, Transition::Linear>>(this->m);
}

this->transitioner->init(period, newPeriod, samples);
this->transitioner2->init(m, newM, samples);
}

template class Mixer::CheapSawtoothGenerator<float>;