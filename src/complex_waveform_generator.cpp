#include <ComplexWaveformGenerator.h>

template <typename T>
Mixer::ComplexWaveformGenerator<T>::ComplexWaveformGenerator(const std::vector<std::pair<float,T>> & components, const unsigned int &channels, const unsigned int &sampleRate)
: AudioSource(channels,sampleRate)
{
    for (auto & component : components){
        auto generator = std::make_unique<Mixer::SineGenerator<T>>(component.first,channels,sampleRate);
        auto volumeControl = std::make_unique<DataFlow::Applicator<T,VolumeControl<T>>>();
        volumeControl->getMethod().setVolume(component.second);
        volumeControl->attach(*generator);
        complexSource.attach(*(volumeControl));        
        componentObjects.insert(std::make_pair(component.first,std::make_pair(std::move(generator),std::move(volumeControl))));
    }
}

template <typename T>
std::vector<T> Mixer::ComplexWaveformGenerator<T>::get(const unsigned int &amount){
    if(componentObjects.size()==0){return std::vector<T>(amount,0);}
    return complexSource.get(amount);
}

template <typename T>
T Mixer::ComplexWaveformGenerator<T>::getComponent(const float & frequency){
    auto it = componentObjects.find(frequency);
    if(it!=componentObjects.end()){
        return componentObjects[frequency].second->getMethod().getVolume();
    }
    return 0;
}

template <typename T>
void Mixer::ComplexWaveformGenerator<T>::setComponent(const float &frequency, const float & volume){
    auto it = componentObjects.find(frequency);
    if(it!=componentObjects.end()){
        componentObjects[frequency].second->getMethod().setVolume(volume);
        return;
    }
    auto generator = std::make_unique<Mixer::SineGenerator<T>>(frequency,getChannels(),getSampleRate());
    auto volumeControl = std::make_unique<DataFlow::Applicator<T,VolumeControl<T>>>();
    volumeControl->getMethod().setVolume(volume);
    volumeControl->attach(*generator);
    complexSource.attach(*(volumeControl));        
    componentObjects.insert(std::make_pair(frequency,std::make_pair(std::move(generator),std::move(volumeControl))));
}

template <typename T>
void Mixer::ComplexWaveformGenerator<T>::setComponent(const float& frequency, const float &volume, const float &seconds, std::string method, const unsigned int &degree){
    auto it = componentObjects.find(frequency);
    if(it!=componentObjects.end()){
        componentObjects[frequency].second->getMethod().setVolume(volume,seconds,getSampleRate()*getChannels(),method,degree);
    }
}

template class Mixer::ComplexWaveformGenerator<float>;