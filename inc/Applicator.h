#pragma once

#include <iSource.h>

namespace DataFlow
{
//! \~english A class that applies a functor of type K to the attached iSource of type T
//! \~hungarian Ez az osztály a K típusú funktorral transzformálja a hozzácsatolt iSource<T> forrást
/*! \~english Must initialize the source pointer with attach()!
    \~hungarian A forrást meg kell adni az attach() metódussal!
*/
template <typename T, typename K>
class Applicator : public DataFlow::iSource<T>
{
  private:
    K filter;
    DataFlow::iSource<T> *source;
  public:
    std::vector<T> get(const unsigned int &amount)
    {
        auto res = source->get(amount);
        filter(res);
        return res;
    }

    /*!
        \~english Attach the parameter as this object's source.
        \~hungarian A paramétert az objektum forrásaként hozzácsatolja.
        \param source \~english the iSource<T> to attach
                        \~hungarian a csatolni kívánt iSource<T>
    */
    void attach(DataFlow::iSource<T> &source)
    {
        this->source = &source;
    }
};
};