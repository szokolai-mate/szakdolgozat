#pragma once

#include <iSource.h>

namespace DataFlow{
/*!
    \~english Interface describing a buffer implementation.
    \~hungarian Pufferekhez használt interfész.
*/
template <typename T>
class iBuffer : public DataFlow::iSource<T>{
public:
    virtual void put(std::vector<T> & data) = 0;

    /*!
    \~english Get the size of stored data.
    \~hungarian A tárolt adat méretét adja vissza.
    \~english \return the size of data
    \~hungarian \return az adat mérete
    */
    virtual int size() = 0;

    virtual ~iBuffer(){}
};
};