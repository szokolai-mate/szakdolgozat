#pragma once

#include <vector>

#include <iSource.h>

namespace DataFlow{
    template <typename T, template <typename> typename K>
    class Consolidator : public DataFlow::iSource<T>{
    private:
        K<T> consolidationMethod;
        std::vector<std::reference_wrapper<iSource<T>>> sources;
    public:
        /*!
        \~english Attach the parameter as this object's sources.
        \~hungarian A paramétert az objektum forrásaihoz hozzácsatolja.
        \param source \~english the iSource<T> to attach
                        \~hungarian a csatolni kívánt iSource<T>
        */
        void attach(DataFlow::iSource<T> & source){
            sources.push_back(source);
        }

        bool detach(DataFlow::iSource<T> & source){
            for(auto it = sources.begin();it!=sources.end();it++){
                if(&((*it).get())==&source){
                    sources.erase(it);
                    return true;
                }
            }
            return false;
        }
        std::vector<T> get(const unsigned int & amount){
            consolidationMethod.init(sources[0].get().get(amount));
            for(int i = 1; i<sources.size();i++){
                consolidationMethod.add(sources[i].get().get(amount));
            }
            return consolidationMethod.get();
        }       
    };
};