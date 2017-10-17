#pragma once

#include <memory>

#include <iSource.h>
#include <AudioSource.h>
#include <Consolidator.h>
#include <ConsolidationMethods.h>
#include <TransitionFunctions.h>
#include <RepeatingBuffer.h>

namespace Mixer
{
/*!
    \~english Provides a repeating pattern defined with a waveform and an amplitude envelope.

    The amplitude envelope is made of 3 segments:
    - the **attack**, where the amplitude goes from 0 to 1
    - the **steady state**, where amplitude is constantly 1
    - the **decay**, where the amplitude goes from 1 to 0

    The attack and decay are calculated using a TransitionFunction for each. They can be set by the template parameters A for attack and D for decay respecively.

    To set the speed of the transitions, use setAttack() and setDecay() with the **percentage** of the note where you want the attack to end or the decay to start.

    An iSource is also needed to be supplied with attach() to give the note it's waveform.

    After providing these 3 attributes, call bakeNote() to fill the internal buffer with the audio data. **This must be done before requesting the audio data!**

    Usage example:


        Mixer::SawtoothGenerator<float> generator(80, 150, 2, 48000);
        Mixer::SimpleNote<float,Transition::Linear,Transition::EaseOut::Cubic> note(0.2f,2,48000);

        note.attach(generator);
        note.setAttack(0.1f);
        note.setDecay(0.12f);

        note.bakeNote();

        std::vector<float> result = note.get(note.size());

    The vector *result* now contains multiplexed audio data that is 0.2 seconds long, has a 80Hz sawtooth waveform, approaches it's maximum volume linearily,
     reaches that maximum volume at 10% of the length (0.02 seconds in this case) and begins the reduction of volume at 12% of the note's length, approaching 0 volume with a cubic ease-out function.

    \~hungarian Egy ismétlődő mintát szolgáltat, amit egy hullámforma és egy amplitúdó-függvény határoz meg.

    Az amplitúdó-függvény 3 részből áll:
    - a **növekvő** (attack), ahol az amplitúdó 0-ból 1-be megy
    - a **szinttartó** (steady state), ahol az amplitúdó konstans 1
    - a **csökkenő** (decay), ahol az amplitúdó 1-ből 0-ba megy

    A növekvő és csökkenő részek egy-egy TransitionFunction -al határozandók meg. Ezek az A és D sablonparaméterekkel adhatóak meg, A a növekvő, D a csökkenő résznek.

    Az átmenetek sebességét a setAttack() és setDecay() metódusokkal adhatjuk meg, a növekvés végét vagy a csökkenés kezdetét a hangjegy hosszának **százalékában**.

    Egy iSource megadása az attach() metódussal szintén szükséges. Ez fogja a hangjegy hullámformáját adni.

    Miután ezt a 3 tulajdonságot megadtuk, a bakeNote() metódussal fel kell töltenünk a belső puffert a hangjegy atadával. **Ezt meg kell tenni mielőtt audió adatot kérnénk az objektumtól!**

    Példa használatra:


        Mixer::SawtoothGenerator<float> generator(80, 150, 2, 48000);
        Mixer::SimpleNote<float,Transition::Linear,Transition::EaseOut::Cubic> note(0.2f,2,48000);

        note.attach(generator);
        note.setAttack(0.1f);
        note.setDecay(0.12f);

        note.bakeNote();

        std::vector<float> result = note.get(note.getLength());

    A *result* vektor tartalma multiplex audió adat, ami 0.2 másodperc hosszú, 80Hz-es fűrészfogas hullámformájú, a maximum hangerejét lineárisan közelíti
     és a hangjegy 10%-ánál (0.02 másodperc) éri el, és a hangerő csökkentését a hangjegy 12%-ánál kezdi meg, ami után köbösen lassulva közelíti a 0-át.
*/
template <typename T, template <typename> typename A , template <typename> typename D>
class SimpleNote : public DataFlow::iSource<T>, public AudioSource
{
  private:
    DataFlow::iSource<T> * generator;

    int lengthInSamples;
    float step = 1.0f/lengthInSamples;
    int position = 0;

    A<T> attack;
    D<T> decay;

    float attackEnd, decayStart;

    bool attackInitialized = false, decayInicialized = false;
    
    DataFlow::RepeatingBuffer<T> bakedNote;
  public:
    /*!
        \~english Attach the parameter as this object's waveform source.
        \~hungarian A paramétert az objektum hullámjel-forrásaként hozzácsatolja.
        \~english \param source the iSource<T> to attach
        \~hungarian \param source a csatolni kívánt iSource<T>
    */
    void attach(DataFlow::iSource<T> &generator)
    {
        this->generator = &generator;
    }

    /*!
        \~english Set the **percentage** of the note where the value will begin it's approach to 1.
        \~hungarian Beállítja azon **százalékot** ahol a hangjegy megkezdi az 1-hez való közeledést.
        \~english \param start the start of the attack in percentage : range is **(0;1)**
        \~hungarian \param start a növelés kezdete **százalékban** : lehetséges értéke **(0;1)**
    */
    void setAttack(const float & end){
        attack.init(0.0f,0.0f,end,1.0f);
        attackEnd = end;
        attackInitialized = true;
    }

    /*!
        \~english Set the **percentage** of the note where the value will begin it's approach to 0.
        \~hungarian Beállítja azon **százalékot** ahol a hangjegy megkezdi a 0-hoz való közeledést.
        \~english \param start the start of the decay in percentage : range is **(0;1)**
        \~hungarian \param start a csökkentés kezdete **százalékban** : lehetséges értéke **(0;1)**
    */
    void setDecay(const float & start){
        decay.init(start,1.0f,1.0f,0);
        decayStart = start;
        decayInicialized = true;
    }

    /*!
        \~english Get the length of the multiplexed note.
        \~hungarian A multiplexált hangjegy hosszát adja vissza.
        \~english \return the length of the note
        \~hungarian \return a hangjegy hossza
    */
    int size(){
        return bakedNote.size();
    }

    /*!
        \~english Pre-bakes the note into an internal buffer.

        Must be called before calling get()!
        \~hungarian Előre legyártja a hangjegyet egy belső pufferbe.

        Meg kell hívni get() előtt!
        \~english \return whether the baking was successful
                \~hungarian \return sikeres volt-e a hangjegy legyártása
    */
    bool bakeNote(){
        if(generator && attackInitialized && decayInicialized){
            bakedNote.clear();
            std::vector<T> res;
            std::vector<T> envelope(lengthInSamples,1.0f);
            float position = 0;
            for(int i = 0; i<lengthInSamples*attackEnd;i++){
                envelope[i]=(attack(position));
                position+=step;
            }
            position = decayStart;
            for(int i = position*lengthInSamples;i<lengthInSamples;i++){
                envelope[i] = decay(position);
                position+=step;
            }
            std::vector<T> waveform = generator->get(lengthInSamples*getChannels());
            int envelopePos = 0;
            for(int i = 0; i < lengthInSamples * getChannels(); i+=getChannels()){
                for (int j = 0 ; j<getChannels();j++){
                    res.push_back( envelope[envelopePos] * waveform[i+j]);
                }
                envelopePos++;
            }
            bakedNote.put(res);
            return true;
        }
        return false;
    }

    /*!
        \~english Clears the internal state, so the next get() will begin with the start of the note.
        \~hungarian Kiüríti a belső állapotot, hogy a következő get() eredménye a hangjegy elejével kezdődjön.
    */
    void clear(){
        position = 0;
    }

    std::vector<T> get(const unsigned int & amount){
        return bakedNote.get(amount);
    }
    
    /*!
        \~english Constructor. The length of the note is static.
        \~hungarian Konstruktor. A hangjegy hossza soha nem változhat.
        \~english \param lengthInSeconds the length of the note in seconds
        \~hungarian \param lengthInSeconds a hangjegy hossza másodpercben
        \~english \param channels the number of audio channels
        \~hungarian \param channels a hangcsatornák száma
        \~english \param sampleRate the sampling rate
        \~hungarian \param sampleRate a mintavételezés sűrűsége
    */
    SimpleNote(const float &lengthInSeconds, const unsigned int &channels, const unsigned int &sampleRate)
    : AudioSource(channels,sampleRate), lengthInSamples(sampleRate*lengthInSeconds){}
};
};