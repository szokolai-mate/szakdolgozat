#include <RepeatingBuffer.h>

template <typename T>
std::vector<T> DataFlow::RepeatingBuffer<T>::get(const unsigned int &amount){
    std::vector<T> res;
    res.reserve(amount);
    for(int i = 0;i<amount;i++){
        res.push_back(buffer[pos]);
        pos=(pos+1)%buffer.size();
    }
    return res;
}

template <typename T>
void DataFlow::RepeatingBuffer<T>::put(std::vector<T> data){
    buffer = std::move(data);
    clear();
}

template <typename T>
void DataFlow::RepeatingBuffer<T>::clear(){
    pos = 0;
}

template <typename T>
int DataFlow::RepeatingBuffer<T>::size(){
    return buffer.size();
}

template <typename T>
DataFlow::RepeatingBuffer<T>::RepeatingBuffer(std::vector<T> data){
    buffer = std::move(data);
}

template class DataFlow::RepeatingBuffer<float>;