#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>

#include <AudioSource.h>
#include <Transitioner.h>
#include <TransitionFunctions.h>
#include <iTransitioner.h>
#include <Consolidator.h>
#include <ConsolidationMethods.h>
#include <VolumeControl.h>
#include <SineGenerator.h>
#include <Applicator.h>

namespace Mixer{
    /*!
        \~english This class will provide a custom waveform by utilizing spectral synthesis.

        Every waveform can be made by combining sinusoids with varying frequencies and amplitudes. These sinusoids are also called components.
         Each set of components defines a unique waveform. This set is also known as the waveform's spectrum.

        If all components of a wave are known, adding the components together will perfectly reproduce the wave. This is known as spectral or Fourier synthesis.

        \~hungarian Ez az osztály egyedi hullámformát szolgáltat, amit spektrális szintézissel készít el.

        Minden hullám előállítható különböző amplitúdójú és frekvenciájú szinuszoidok kombinációjaként. Ezeket a szinuszoidokat a hullám komponenseinek is nevezik.
         Minden komponens-halmaz egy egyedi hullámot határoz meg. Ez a halmaz a hullám spektruma.

        Ha egy hullámnak minden komponense ismert, ezek összeadásával az eredeti hullám tökéletezen replikálható. Ezt a teknikát hívják spektrális vagy Fourier szintézisnek.
    */
    template <typename T>
    class ComplexWaveformGenerator : public DataFlow::iSource<T>, public Mixer::AudioSource{
    private:
        float frequency;

        DataFlow::Consolidator<T, Consolidation::Accumulation> complexSource;        

        std::map<float,std::pair<std::unique_ptr<Mixer::SineGenerator<T>>,std::unique_ptr<DataFlow::Applicator<T,VolumeControl<T>>>>> componentObjects;
    public:
        std::vector<T> get(const unsigned int &amount);
        
        /*!
            \~english Returns the volume of a selected frequency.
            \~hungarian A megadott frekvencia hangerejét adja vissza.
            \~english \param frequency the frequency of the component
            \~hungarian \param frequency a komponens frekvenciája
            \~english \return the volume of the component
            \~hungarian \return a komponens hangereje
        */
        T getComponent(const float & frequency);
        
    /*!
      \~english Set the volume of a component or add a new component.
      \~hungarian Egy komponens hangerejének beállítása, vagy új komponens hozzáadása.
  
      \~english \param frequency the frequency of the component
      \~hungarian \param frequency a komponens frekvenciája
      
      \~english \param volume the desired volume
      \~hungarian \param volume a kívánt hangerő
    */
        void setComponent(const float &frequency, const float & volume);
        
        /*!
      \~english Set the volume of the component with the desired transition. The waveform must already contain this frequency.
      \~hungarian Beállítja a komponens hangerejét a kívánt átmenettel. A frekvenciának már a hullám komponensének kell lennie.
  
      \~english \param frequency the frequency of the component to change
      \~hungarian \param frequency a módosítani kívánt komponens frekvenciája
      

      \~english \param volume the desired volume
      \~hungarian \param volume a kívánt hangerő
      
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
        void setComponent(const float &frequency, const float &volume, const float &seconds, std::string method = "linear", const unsigned int &degree = 2);
        
        /*!
            \~english Constructor. Requires the sinusoid components and AudioSource parameters.
            \~hungarian Konstruktor. Beállítja a szinuszoid komponenseket és az AudioSource paramétereit.
            \~english \param components a vector of frequency and volume pairs, each pair describes a sinusoid
                                \~hungarian \param components egy frekvenciából és hangerőből álló párok vektora, minden ilyen pár leír egy szinuszoid komponenst
            \~english \param channels the channel count
                                \~hungarian \param channels a csatornák száma
            \~english \param sampleRate the sample rate
                                \~hungarian \param sampleRate a mintavételezés sűrűsége
        */
        ComplexWaveformGenerator(const std::vector<std::pair<float,T>> & components, const unsigned int &channels, const unsigned int &sampleRate);
    };
};