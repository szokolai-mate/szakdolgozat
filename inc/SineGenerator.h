#pragma once

#include <math.h>
#include <vector>
#include <memory>

#include <iSource.h>
#include <AudioSource.h>
#include <Transitioner.h>
#include <TransitionFunctions.h>
#include <iTransitioner.h>
#include <Utils.h>

#define SINE_TABLE_SIZE 4800000

namespace Mixer
{
/*!
  \~english A source that provides a continous sine wave with the set frequency.
  \~hungarian Egy megadott frekvenciájú szinusz hullámot szolgáltató forrás.
*/
template <typename T>
class SineGenerator : public DataFlow::iSource<T>, public Mixer::AudioSource
{
private:
  float step;
  unsigned int position;
  static bool sinetableUninitialized;

  static constexpr float filler(float x) { return (float)sin(((double)x / (double)SINE_TABLE_SIZE) * M_PI * 2.0); }
  static std::array<float, SINE_TABLE_SIZE> sinetable;

  std::unique_ptr<iTransitioner<T>>  transitioner;

public:
  std::vector<T> get(const unsigned int &amount);

  /*!
    \~english Get the current frequency.
    \~hungarian Visszaadja a jelenlegi frekvenciát.

    \~english \return the current frequency
    \~hungarian \return a jelenlegi frekvencia
  */
  float getFrequency() const;

  /*!
    \~english Set the frequency without any transition.
    \~hungarian Beállítja a frekvenciát átmenet nélkül.

    \~english \param frequency the desired frequency
    \~hungarian \param frequency a kívánt frekvencia
  */
  void setFrequency(const float & frequency);

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
  void setFrequency(const float & frequency, const float & seconds, std::string method = "linear", const unsigned int & degree = 2);

  /*!
    \~english Constructor. Sets initial frequency and requires AudioSource parameters.
    \~hungarian Konstruktor. Beállítja a kezdeti frekvenciát és az AudioSource paramétereit.
    \~english \param frequency the initial frequency
                      \~hungarian \param frequency a kezdeti frekvencia
    \~english \param channels the channel count
                      \~hungarian \param channels a csatornák száma
    \~english \param sampleRate the sample rate
                      \~hungarian \param sampleRate a mintavételezés sűrűsége
  */
  SineGenerator(const float &frequency, const unsigned int &channels, const unsigned int &sampleRate);
};
};