#pragma once

#include <string>
#include <vector>
#include <memory>

#include <iWaveformGenerator.h>
#include <AudioSource.h>
#include <Transitioner.h>
#include <TransitionFunctions.h>
#include <iTransitioner.h>

namespace Mixer
{
/*!
  \~english A source that provides a continous sawtooth wave with the set frequency.


  **WARNING!**

  This generator is extremely efficient but the frequency is imprecise.

  \~hungarian Egy megadott frekvenciájú fűrészfog hullámot szolgáltató forrás.

   **FIGYELEM!**

    Ez a generator rednkívül hatékony, viszont pontatlan a frekvenciája.

*/
template <typename T>
class CheapSawtoothGenerator : public Mixer::iWaveformGenerator<T>, public Mixer::AudioSource
{
private:
    float frequency;
    
    float period;
    float m;
    float position = 0;
    bool putOnes = true;

    std::unique_ptr<iTransitioner<T>>  transitioner;    
    std::unique_ptr<iTransitioner<T>>  transitioner2;    
public:

    std::vector<T> get(const unsigned int &amount);
    
    float getFrequency() const;
    
    void setFrequency(const float &frequency);
    
    void setFrequency(const float &frequency, const float &seconds, std::string method = "linear", const unsigned int &degree = 2);

    /*!
        \~english Constructor. Sets initial properties and requires AudioSource parameters.
        \~hungarian Konstruktor. Beállítja a kezdeti tulajdonságokat és az AudioSource paramétereit.
        \~english \param frequency the initial frequency
                            \~hungarian \param frequency a kezdeti frekvencia
        \~english \param channels the channel count
                            \~hungarian \param channels a csatornák száma
        \~english \param sampleRate the sample rate
                            \~hungarian \param sampleRate a mintavételezés sűrűsége
        */
    CheapSawtoothGenerator(const float &frequency, const unsigned int &channels, const unsigned int &sampleRate);
};
};