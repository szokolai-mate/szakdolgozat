#include <SquareGenerator.h>

template <typename T>
Mixer::SquareGenerator<T>::SquareGenerator(const float &frequency, const unsigned int & harmonics,const unsigned int &channels, const unsigned int &sampleRate) : AudioSource(channels, sampleRate) , frequency(frequency) , harmonics(harmonics)
{
    generators.reserve(harmonics+1);
    volumeControllers.reserve(harmonics+1);
    for(int i = 1;i<harmonics*2+2;i+=2){
        generators.push_back(std::make_unique<Mixer::SineGenerator<T>>(frequency*i,channels,sampleRate));
        volumeControllers.push_back(std::make_unique<DataFlow::Applicator<T,VolumeControl>>());
        volumeControllers.back()->getMethod().setVolume(1.0f/(float)i);
        volumeControllers.back()->attach(*(generators.back()));
        complexSource.attach(*(volumeControllers.back()));
    }
}

template <typename T>
std::vector<T> Mixer::SquareGenerator<T>::get(const unsigned int &amount)
{
   return complexSource.get(amount);
}

template <typename T>
float Mixer::SquareGenerator<T>::getFrequency() const
{
    return frequency;
}

template <typename T>
void Mixer::SquareGenerator<T>::setFrequency(const float &frequency)
{
    int i = 1;
    for(auto & sg : generators){
        sg->setFrequency(frequency*i);
        i++;
    }
    this->frequency = frequency;
}

template <typename T>
void Mixer::SquareGenerator<T>::setFrequency(const float &frequency, const float &seconds, std::string method, const unsigned int &degree)
{
    int i = 1;
    for(auto & sg : generators){
        sg->setFrequency(frequency*i,seconds,method,degree);
        i++;
    }
    this->frequency = frequency;
}

template class Mixer::SquareGenerator<float>;