#include <ConsolidationMethods.h>

template <typename T>
bool Consolidation::Accumulation<T>::init(const std::vector<T> & initial){
    workspace = std::move(initial);
    return true;
}

template <typename T>
bool Consolidation::Accumulation<T>::add(const std::vector<T> & vector){
    for(int i = 0; i<vector.size();i++){
        workspace[i]+=vector[i];
    }
}

template <typename T>
std::vector<T> Consolidation::Accumulation<T>::get(){
    return std::move(workspace);
}

template class Consolidation::Accumulation<float>;
template class Consolidation::Accumulation<int>;

template <typename T>
bool Consolidation::Multiplication<T>::init(const std::vector<T> & initial){
    workspace = std::move(initial);
    return true;
}

template <typename T>
bool Consolidation::Multiplication<T>::add(const std::vector<T> & vector){
    for(int i = 0; i<vector.size();i++){
        workspace[i]*=vector[i];
    }
}

template <typename T>
std::vector<T> Consolidation::Multiplication<T>::get(){
    return std::move(workspace);
}

template class Consolidation::Multiplication<float>;
template class Consolidation::Multiplication<int>;