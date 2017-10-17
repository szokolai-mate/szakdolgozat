#pragma once

#include <iBuffer.h>

namespace DataFlow{
/*!
    \~english A simple iBuffer that supplies an infinitely repeating pattern of the stored data.

    This class is **not** for normal buffering! It will ony make one data block into an infinite pattern.
    \~hungarian Egy egyszerű iBuffer ami a tárolt adat végtelenszeres ismétlődésének megfelelő adatot szolgáltat.

    Ez az osztály **nem** szokásos pufferelésre használandó! Csak egy adatblokk végtelenszeresítésére való.
*/
template <typename T>
class RepeatingBuffer : public DataFlow::iBuffer<T>{
private:
    int pos = 0;
    std::vector<T> buffer;
public:
    std::vector<T> get(const unsigned int &amount);

    /*!
        \~english **Replace** the contents of this buffer with the argument.
        \~hungarian A puffer tartalmát **kicseréli** az argumentumra.
        \~english \param data the new buffered data
        \~hungarian \param data az új pufferelni kívánt adat
    */
    void put(std::vector<T> data);

    /*!
        \~english Clear the internal state of this object.
        
        The next get() will begin with the start of the buffered data.

        \~hungarian Törli az objektum belső állapotát.

        A következő get() eredménye a pufferelt adat elejével fog kezdődni.
    */
    void clear();
    int size();

    /*!
        \~english Default constructor.
        \~hungarian Alapértelmezett konstruktor.
    */
    RepeatingBuffer(){}

    /*!
        \~english Constructor. Initialized buffer with the argument.
        \~hungarian Konstruktor. Inicializálja a puffert az argumentummal.
        \~english \param data the data to buffer
        \~hungarian \param data a pufferelni kívánt adat
    */
    RepeatingBuffer(std::vector<T> data);
};
};