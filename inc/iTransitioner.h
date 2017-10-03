#pragma once


//! \~english Interface defining the functionality of the Transitioner.
//! \~hungarian A Transitioner funkcionalitását definiáló interfész.
/*!
    \~english Use an iTransitioner pointer as data member in order to switch TransitionFunction at runtime. For an example see SineGenerator.
    \~hungarian Adattagként egy iTransitioner pointer használata szükséges ahhoz hogy TransitionFunction -t futási időben válasszunk. Példaként lásd SineGenerator.
*/
template <typename T>
class iTransitioner{
public:
    /*!
        \~english Step the internal state forward on the transition function, changing the attached variable.
        \~hungarian Előre lépteti az átmenet függvény belső állapotát, a csatolt változót megfelelően változtatva.
        \~english \param amount the amount to step with
        \~hungarian \param amount a lépés mértéke
    */
    virtual void step(const float & amount) = 0;

    /*!
        \~english Initialize the internal state of the transition function.
        \~hungarian Az átmenet függvény belső állapotának kezdeti felépítése.
        \~english \param startValue the start value, where we transition **from**
        \~hungarian \param startValue a kezdeti érték, **ahonnan** az érték megy át
        \~english \param endValue the end value, where we transition **to**
        \~hungarian \param endValue a végső érték, **ahova** az érték megy át
        \~english \param endTime the position where value will reach endValue
        \~hungarian \param endTime a pozíció ahol az érték eléri a végső értéket
    */
    virtual void init(const T & startValue, const T & endValue, const float & endTime) = 0;
    
    virtual ~iTransitioner(){}
};