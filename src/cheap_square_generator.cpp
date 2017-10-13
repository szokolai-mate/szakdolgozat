#include <CheapSquareGenerator.h>

template <typename T>
Mixer::CheapSquareGenerator<T>::CheapSquareGenerator(const float &frequency, const unsigned int &channels, const unsigned int &sampleRate) : AudioSource(channels, sampleRate) , frequency(frequency)
{
    halfPeriod = (getSampleRate() / (frequency * 2.0f));
}

template <typename T>
std::vector<T> Mixer::CheapSquareGenerator<T>::get(const unsigned int &amount)
{
   std::vector<T> res(amount,0);
   for(int i = 0;i<amount;i+=getChannels()){
       if(putOnes){
           for(int j = 0;j<getChannels();j++)
           {
            res[i+j]=1;
           }
       }
       else{
        for(int j = 0;j<getChannels();j++)
        {
            res[i+j]=-1;
        }
       }
       counter+=getChannels();
       if (transitioner)
       {
           transitioner->step(1);
       }
       if(counter>=halfPeriod*getChannels()){
           counter = 0;
           putOnes=!putOnes;
        }
   }

   return res;
}

template <typename T>
float Mixer::CheapSquareGenerator<T>::getFrequency() const
{
    return frequency;
}

template <typename T>
void Mixer::CheapSquareGenerator<T>::setFrequency(const float &frequency)
{
    this->frequency = frequency;
    halfPeriod = (getSampleRate() / (frequency * 2.0f));
}

template <typename T>
void Mixer::CheapSquareGenerator<T>::setFrequency(const float &frequency, const float &seconds, std::string method, const unsigned int &degree)
{
    this->frequency = frequency;
    float newHalfPeriod = (getSampleRate() / (frequency * 2.0f));
    float samples = seconds * getSampleRate();

    if ("linear" == method)
    {
        this->transitioner = std::make_unique<Transitioner<T, Transition::Linear>>(this->halfPeriod);
    }
    switch (degree)
    {
    case 2:
        if ("ease-in-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Quadratic>>(this->halfPeriod);
            break;
        }
        if ("ease-in" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Quadratic>>(this->halfPeriod);
            break;
        }
        if ("ease-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Quadratic>>(this->halfPeriod);
            break;
        }

    case 3:
        if ("ease-in-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Cubic>>(this->halfPeriod);
            break;
        }
        if ("ease-in" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Cubic>>(this->halfPeriod);
            break;
        }
        if ("ease-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Cubic>>(this->halfPeriod);
            break;
        }

    case 4:
        if ("ease-in-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Quartic>>(this->halfPeriod);
            break;
        }
        if ("ease-in" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Quartic>>(this->halfPeriod);
            break;
        }
        if ("ease-out" == method)
        {
            this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Quartic>>(this->halfPeriod);
            break;
        }

    case 5:
            if ("ease-in-out" == method)
            {
                this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Quintic>>(this->halfPeriod);
                break;
            }
            if ("ease-in" == method)
            {
                this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Quintic>>(this->halfPeriod);
                break;
            }
            if ("ease-out" == method)
            {
                this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Quintic>>(this->halfPeriod);
                break;
            }
    default:
        this->transitioner = std::make_unique<Transitioner<T, Transition::Linear>>(this->halfPeriod);
    }

    this->transitioner->init(halfPeriod, newHalfPeriod, samples);
}

template class Mixer::CheapSquareGenerator<float>;