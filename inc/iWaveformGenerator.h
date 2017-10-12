#pragma once

#include <string>

#include <iSource.h>

namespace Mixer
{
template <typename T>
class iWaveformGenerator : public DataFlow::iSource<T>
{
  public:
    /*!
    \~english Get the current frequency.
    \~hungarian Visszaadja a jelenlegi frekvenciát.

    \~english \return the current frequency
    \~hungarian \return a jelenlegi frekvencia
  */
    virtual float getFrequency() const = 0;

    /*!
    \~english Set the frequency without any transition.
    \~hungarian Beállítja a frekvenciát átmenet nélkül.

    \~english \param frequency the desired frequency
    \~hungarian \param frequency a kívánt frekvencia
  */
    virtual void setFrequency(const float &frequency) = 0;

    /*!
      \~english Set the frequency with the desired transition.
      \~hungarian Beállítja a frekvenciát a kívánt átmenettel.
  
      \~english \param frequency the desired frequency
      \~hungarian \param frequency a kívánt frekvencia
      
      \~english \param seconds the length of transition in seconds
      \~hungarian \param seconds az átmenet hossza másodpercben
      
      \~english \param method the method of transition
  
      possible values:
      - linear
      - ease-in
      - ease-out
      - ease-in-out
  
      Possibly more values in Transition namespace.
      
      \~hungarian \param method az átmenet jellege
  
      lehetséges értékek:
      - linear
      - ease-in
      - ease-out
      - ease-in-out
  
      Lehetséges hogy több érték van. Lásd Transition névtér.
  
      \~english \param degree the exponential degree of the transition method (if applicable)
      \~hungarian \param degree az átmenet hatványkitevője (ha lehetséges)
      
    */
    virtual void setFrequency(const float &frequency, const float &seconds, std::string method = "linear", const unsigned int &degree = 2) = 0;

    virtual ~iWaveformGenerator() {}
};
};