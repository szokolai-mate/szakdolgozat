#include <SineGenerator.h>

template <typename T>
const std::array<float,Mixer::SineGenerator<T>::table_size> Mixer::SineGenerator<T>::sinetable = constexpr_array::make<Mixer::SineGenerator<T>::table_size>(Mixer::SineGenerator<T>::functor);

template <typename T>
Mixer::SineGenerator<T>::SineGenerator(const float& frequency){
    this->step=frequency/20;
    this->position = 0;
}
template <typename T>
std::vector<T> Mixer::SineGenerator<T>::get(const unsigned int & amount){
    //TMP: adds 2 of the same, for stereo duplexing
    std::vector<T> res;
    for (unsigned int count = 0; count<amount;count++){
        res.push_back(sinetable[position]);
        res.push_back(sinetable[position]);
        position=(int)round((position+step)) % SINE_TABLE_SIZE;
    }
    return res;
}

template class Mixer::SineGenerator<float>;