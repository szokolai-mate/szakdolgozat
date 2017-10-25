#include <QueueBuffer.h>

template <typename T>
std::vector<T> DataFlow::QueueBuffer<T>::get(const unsigned int &amount){
    std::vector<T> res;
    res.reserve(amount);
    for(int i = 0;i<amount;i++){
        res.push_back(buffer.front());
        buffer.pop();
    }
    return res;
}

template <typename T>
T DataFlow::QueueBuffer<T>::get(){
    auto res = buffer.front();
    buffer.pop();
    return res;
}

template <typename T>
void DataFlow::QueueBuffer<T>::put(std::vector<T> data){
    for (T & e : data){
        buffer.push(std::move(e));
    }
}

template <typename T>
void DataFlow::QueueBuffer<T>::put(T element){
    buffer.push(std::move(element));
}

template <typename T>
void DataFlow::QueueBuffer<T>::clear(){
    std::queue<T> empty;
    std::swap(buffer,empty);
}

template <typename T>
int DataFlow::QueueBuffer<T>::size(){
    return buffer.size();
}

template class DataFlow::QueueBuffer<float>;