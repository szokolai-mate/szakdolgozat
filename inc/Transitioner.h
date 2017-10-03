#pragma once

#include <iTransitioner.h>
/*!

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
    }

    void step(const float & amount){
        if(position>=endTime){
            variable = endValue;
            return;
        }
        position+=amount;
        variable = functor(position);
    }

    /*!

    */
    Transitioner(T & referenceToVariable) : variable(referenceToVariable){}
};