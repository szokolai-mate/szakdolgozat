#pragma once

#include <math.h>
#include <vector>

#include <iSource.h>
#include <AudioSource.h>
#include <Utils.h>
//! \todo TODO: változtatható frekvencia
//! \todo TODO: actually use sample rate

//TODO: NOTE: sample rate is fixed to 48000
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

public:
  std::vector<T> get(const unsigned int &amount);

  /*!
    \~english Constructor. Sets initial frequency and requires AudioSource parameters.
    \~hungarian Konstruktor. Beállítja a kezdeti frekvenciát és az AudioSource paramétereit.
    \~english \param frequency the initial frequency
                      \~hungarian \param frequency a kezdeti frekvencia
    \~english \param channels the channel count
                      \~hungarian \param channels a csatornák száma
    \~english \param sampleRate the sample rate
                      \~hungarian \param sampleRate a intavételezés sűrűsége
  */
  SineGenerator(const float &frequency, const unsigned int &channels, const unsigned int &sampleRate);
};
};