#include <ConsolidationMethods.h>

template <typename T>
bool Consolidation::Accumulation<T>::init(const std::vector<T> & initial){
    workspace = initial;
    return true;
}

template <typename T>
bool Consolidation::Accumulation<T>::add(const std::vector<T> & vector){
    for(int i = 0; i<vector.size();i++){
        workspace[i]+=vector[i];
        workspace[i] = workspace[i];
    }
}

template <typename T>
std::vector<T> Consolidation::Accumulation<T>::get(){
    return std::move(workspace);
}

template class Consolidation::Accumulation<float>;
template class Consolidation::Accumulation<int>;
template class Consolidation::Accumulation<unsigned int>;