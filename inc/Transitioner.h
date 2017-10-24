#pragma once

#include <iTransitioner.h>
//! \~english Class for transitioning the attached variable according to the TransitionFunction.
//! \~hungarian Az addot TransitionFunction alapján a csatolt változó átmenetére használt osztály.
/*!
 \~english The variable must be passed as reference to the constructor.

 To change the value of the variable use step(). This will move the internal state forward on the transition function.

 The TransitionFunction to use is the template parameter K. See the Transition namespace for implementations.

 \~hungarian A változó referenciaként adandó a konstruktornak.

 A változó értékének változtatásához a step() metódus használandó. Ez a belső állapotot lépteti előre az átmeneti függvényen.

 A használandó TransitionFunction a K sablon paraméter. Implementációkhoz lásd a Transition névteret.
*/
template <typename T, template <typename>typename K>
class Transitioner : public iTransitioner<T>{
private:
    T & variable;
    K<T> functor;

    T startValue, endValue;
    float endTime;
    float position = 0;

public:
    void init(const T & startValue, const T & endValue, const float & endTime){
        this->startValue = startValue;
        this->endValue = endValue;
        this->endTime = endTime;
        functor.init(0,startValue,endTime,endValue);
        variable = startValue;
    }

    void step(const float & amount){
        position+=amount;        
        if(position>=endTime){
            variable = endValue;
            return;
        }
        if(position<=0){
            variable = startValue;
            return;
        }
        variable = functor(position);
    }

    /*!
        \~english Constructor.
        \~hungarian Konstruktor.

        \~english \param referenceToVariable the reference to the variable to transition
        \~hungarian \param referenceToVariable a referencia az átvivendő változóhoz
    */
    Transitioner(T & referenceToVariable) : variable(referenceToVariable){}
};