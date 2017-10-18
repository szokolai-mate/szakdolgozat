#include <Multiplexer.h>

template <typename T>
std::vector<T> Mixer::Multiplexer<T>::get(const unsigned int &amount){
    std::vector<T> res;
    res.reserve(amount);
    std::vector<std::vector<T>> vectors;
    for(int j = 1; j<getChannelCount()+1;j++){
        auto it = sources.find(j);
        if(it==sources.end()){
            vectors.push_back(std::vector<T>(amount/getChannelCount(),0));
        }
        else{
            vectors.push_back(sources[j]->get(amount/getChannelCount()));
        } 
    }
    for(int i = 0; i<amount/getChannelCount();i++){
        for(int j = 0; j<getChannelCount();j++){
            res.push_back(vectors[j][i]);
        }
    }
    return res;
}

template <typename T>
void Mixer::Multiplexer<T>::clearChannel(const unsigned int & channel){
    auto it = sources.find(channel);
    if(it!=sources.end()){
        sources.erase(it);
    }
}

template <typename T>
void Mixer::Multiplexer<T>::setChannel(const unsigned int & channel, DataFlow::iSource<T> & source){
    sources[channel] = &source;
}

template class Mixer::Multiplexer<float>;