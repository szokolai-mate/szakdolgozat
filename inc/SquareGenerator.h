#pragma once

#include <string>
#include <vector>
#include <memory>

#include <iWaveformGenerator.h>
#include <AudioSource.h>
#include <Transitioner.h>
#include <TransitionFunctions.h>
#include <iTransitioner.h>
#include <Consolidator.h>
#include <ConsolidationMethods.h>
#include <VolumeControl.h>
#include <SineGenerator.h>
#include <Applicator.h>

namespace Mixer
{
/*!
  \~english A source that provides a continous square wave with the set frequency.

  The **harmonics** constructor parameter determines the quality of the waveform.
  \~hungarian Egy megadott frekvenciájú négyzetes hullámot szolgáltató forrás.

  A **harmonics** konstruktor paraméter szabályozza a hullámjel minőségét.
*/
template <typename T>
class SquareGenerator : public Mixer::iWaveformGenerator<T>, public Mixer::AudioSource
{
private:
private:
    float frequency;
    unsigned int harmonics;
    DataFlow::Consolidator<T, Consolidation::Accumulation> complexSource;

    std::vector<std::unique_ptr<Mixer::SineGenerator<T>>> generators;
    std::vector<std::unique_ptr<DataFlow::Applicator<T,VolumeControl>>> volumeControllers;
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
        \~english \param harmonics the number of harmonic frequencies
                            \~hungarian \param harmonics a harmónikus frekvenciák száma
        \~english \param channels the channel count
                            \~hungarian \param channels a csatornák száma
        \~english \param sampleRate the sample rate
                            \~hungarian \param sampleRate a mintavételezés sűrűsége
        */
    SquareGenerator(const float &frequency, const unsigned int &harmonics, const unsigned int &channels, const unsigned int &sampleRate);
};
};