#pragma once

#include <vector>

#include <iSource.h>

namespace DataFlow{
    //! \~english Consolidates multiple sources into a single one.
    //! \~hungarian Több forrást egy forrássá alakít.
    /*!
        \~english The template parameter K is the method with which the source consolidation is performed.
                  The type of consolidation must match the type of the consolidator.
                  See the Consolidation namespace for implemented methods.
        \~hungarian A konszolidáció a K sablon paraméter szerint történik.
                    A konszolidátor és a konszolidáció típusának meg kell egyeznie a sablonban.
                    Implementált konszolidációs moódszerekhez lásd a Consolidation névteret.
    */
    template <typename T, template <typename> typename K>
    class Consolidator : public DataFlow::iSource<T>{
    private:
        K<T> consolidationMethod;
        std::vector<std::reference_wrapper<iSource<T>>> sources;
    public:
        /*!
        \~english Attach the parameter to this object's sources.
        \~hungarian A paramétert az objektum forrásaihoz hozzácsatolja.
        \~english \param source the iSource<T> to attach
                        \~hungarian \param source a csatolni kívánt iSource<T>
        */
        void attach(DataFlow::iSource<T> & source){
            sources.push_back(source);
        }
        
        /*!
        \~english Remove the parameter from this object's sources.
        \~hungarian A paramétert az objektum forrásaiból kiveszi.
        \~english \param source the iSource<T> to detach
                        \~hungarian \param source a kivenni kívánt iSource<T>
        */
        bool detach(DataFlow::iSource<T> & source){
            for(auto it = sources.begin();it!=sources.end();it++){
                if(&((*it).get())==&source){
                    sources.erase(it);
                    return true;
                }
            }
            return false;
        }

        //! \~english Get a reference to the consolidation method instance used.
        //! \~hungary Egy referenciát ad a használt konszolidációs módszer példányhoz.
        /*! \~english \return the reference
                    \~hungarian \return a referencia
        */
        K<T> & getMethod(){
            return consolidationMethod;
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