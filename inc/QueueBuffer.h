#pragma once

#include <queue>

#include <iBuffer.h>

namespace DataFlow{
/*!
   \~english A First In First Out iBuffer. The data supplied is a continous stream made up of the data put() inside.
   \~hungarian Egy FIFO (első be, első ki) iBuffer. A szolgáltatott adat egy folytonos folyam, ami a put() -al berakott adatokat tartalmazza helyes időrendben.
*/
template <typename T>
class QueueBuffer : public DataFlow::iBuffer<T>{
private:
    std::queue<T> buffer;
public:
    std::vector<T> get(const unsigned int &amount);

    /*!
        \~english Get a single element from this buffer.
        \~hungarian Egy elemet kér a buffertől.
        \~english \return the next element
        \~hungarian \return a következő elem
    */
    T get();

    /*!
        \~english Add the contents of the argument to this buffer.
        \~hungarian Az argumentum tartalmát berakja a pufferbe.
        \~english \param data the data to add
        \~hungarian \param data a pufferelni kívánt adat
    */
    void put(std::vector<T> data);

     /*!
        \~english Add a single element to this buffer.
        \~hungarian Az argumentum elemet berakja a pufferbe.
        \~english \param element the element to add
        \~hungarian \param element a pufferelni kívánt elem
    */
    void put(T element);

    /*!
        \~english Clear the internal state of this object, removing all buffered data.
        \~hungarian Törli az objektum belső állapotát, ezzel kiürítve a puffert.
    */
    void clear();
    
    int size();
};
};