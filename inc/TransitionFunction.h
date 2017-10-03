#pragma once

//! \~english Abstract base class for a functor class that constructs a transition function between two points.
//! \~hungarian Absztrakt ősosztálya a két pont közötti átmenet-függvényeket konstruáló funktoroknak.
/*!
    \~english See the Transition namespace for implementations.
    \~hungarian Implementációkhoz lásd a Transition névteret.
*/
template <typename T>
class TransitionFunction{
protected:
    float m;
    float x1;
    float x2;
    T y1;
    T y2;
    TransitionFunction(){};
public:
    /*!
        \~english Initialize the functor, constructing the desired function between the two given points.
        \~hungarian Inicializálja a funktort, ezzel felépítve a két adott pont közötti átmeneti függvényt.

        \~english \param x1,y1 coordinates for the starting point
        \~english \param x2,y2 coordinates for the end point
        
        \~hungarian \param x1,y1 a kezdőpont koordinátái
        \~hungarian \param x2,y2 a végpont koordinátái
    */
    virtual void init(const float & x1, const T & y1, const float & x2, const T & y2) = 0;

    /*!
        \~english Get the value at the given position.
        \~hungarian Megadja az adott helyen a függvény értékét.

        \~english \param x the position
        \~hungarian \param x a pozíció

        \~english \return the value at **x**
        \~hungarian \return a **x**-beli érték
    */
    virtual T operator()(const float & x) = 0;

    virtual ~TransitionFunction() {};
};