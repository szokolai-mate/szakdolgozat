#include "iLoader.h"
#include "utils.h"

template <typename T>
class dummyLoader : iLoader<T>{
public:
    void load(iBuffer<T> & _buffer , std::promise<audio_descriptor> _adp);
    bool open(const std::string & _filename);
    void pause_load();
private:
    T counter = 0;
    bool loading = true;
};

template <typename T>
void dummyLoader<T>::load(iBuffer<T> &_buffer, std::promise<audio_descriptor> _adp) {
    audio_descriptor ad(2,48000);
    _adp.set_value(ad);

    while(loading){
        if(!(_buffer.isFull())){
            _buffer.add(counter);
            counter++;
        }
        //else break;
    }
}


template <typename T>
bool dummyLoader<T>::open(const std::string & _filename){}

template <typename T>
void dummyLoader<T>::pause_load() {
    this->loading=false;
}