#pragma once

#include <vector>

#include <QueueBuffer.h>

namespace Mixer{
/*!
    \~english Namespace containing functors which have some kind of effect when applied to audio data.

    Made for usage with the Applicator class.
    \~hungarian Olyan funktorokat tartamlazó névtér, melyek valamilyen hatással vannak audió adatra.

    Az Applicator osztállyal való használatra.
*/
namespace Filter{
/*!
    \~english A functor that introduces distortion to audio data by increasing amplitude way above the treshold.

    The degree of amplification can be set with setDegree()

    \~hungarian Egy funktor, mely torzítja az audió adatot az amplitúdó sokszorosításával.

    A torzítás mértéke a setDegree() metódussal állítható.
*/
template <typename T>
class Fuzzbox{
private:
    unsigned int degree = 4;

    inline T amp(const T & x){
        float output, temp;
        if( x < 0.0 )
        {
            temp = x + 1.0f;
            output = (temp * temp * temp) - 1.0f;
        }
        else
        {
            temp = x - 1.0f;
            output = (temp * temp * temp) + 1.0f;
        }
        return output;
    }
public:
    /*!
        \~english Apply distortion to a single value.
        \~hungarian Egy értéket torzít.
        \~english \param x the value to distort
        \~hungarian \param x a torzítandó érték
        \~english \return the distorted value
        \~hungarian \return a torzított érték
    */
    inline T operator()(const T & x){
        if(this->degree<=0){return x;}     
        float output = x;
        for(int i = 0; i<degree;i++){
            output=amp(output);
        }   
        return output;
    }

    /*!
        \~english Apply the distortion effect to a vector of values.
        \~hungarian Egy vektorban kapott összes értéket torzítja.
        \~english \param vec the vector to distort
        \~hungarian \param vec a torzítandó vektor
    */
    void operator()(std::vector<T> & vec){
        for (auto & e : vec){
            e = this->operator()(e);
        }
    }

    /*!
        \~english Set the degree of distortion.
        \~hungarian Beállítja a torzítás fokát.
        \~english \param degree the new degree of distortion
        \~hungarian \param degree a kívánt torzítási fok
    */
    void setDegree(const unsigned int & degree){
        this->degree = degree;
    }

    /*!
        \~english Get the degree of distortion.
        \~hungarian Visszaadja a torzítás fokát.
        \~english \return the degree of distrotion
        \~hungarian \return a torzítás jelenlegi foka
    */
    unsigned int getDegree(){
        return degree;
    }

    /*!
        \~english Default constructor.
        
        The default degree of distortion is 4.
        \~hungarian Alapértelmezett konstruktor.

        Az alapértelmezett torzítási fok a 4.
    */
    Fuzzbox(){}

    /*!
        \~english Constructor. Initializes the degree of distortion.
        \~hungarian Konstruktor. Inicializálja a torzítás fokát.
        \~english \param degree the new degree of distortion
        \~hungarian \param degree a kívánt torzítási fok
    */
    Fuzzbox(unsigned int & degree) : degree(degree){}
};

/*!
    \~english A functor that applies an echo effect to the audio data.

    Use setDelay() to get the desired delay.

    Use setReflectionQuotient() to set how much of the kinetic energy that hits a reflective surface should be reflected.

    \~hungarian Egy funktor, mely visszhang effektet rak az audió adara.

    Az visszhang késleltetésének megadására a setDelay() metódus használható.

    A visszaverés mértékének megadására a setReflectionQuotient() metódus használható.
*/
template <typename T>
class Echo{
private:
    unsigned int delay = 0;
    float reflectionQuotient = 0;

    DataFlow::QueueBuffer<T> buffer;

    unsigned int delayCounter = 0;
public:
    /*!
        \~english Apply the echo effect to a single value.
        \~hungarian Egy értékre alkalmazza a visszhang effektet.
        \~english \param x the value
        \~hungarian \param x az érték
        \~english \return the value with the effect applied
        \~hungarian \return a módosított érték
    */
    inline T operator()(const T & x){
        if(delay>delayCounter){
            delayCounter++;
            buffer.put(x);
            return x;
        }
        auto e = buffer.get()*reflectionQuotient + x;
        buffer.put(e);
        return e;
    }

    /*!
        \~english Apply the echo effect to a vector of values.
        \~hungarian Egy vektorban kapott összes rtékre alkalmazza a visszhang effektet.
        \~english \param vec the vector to apply the effect to
        \~hungarian \param vec a módosítandó vektor
    */
    void operator()(std::vector<T> & vec){
        for (auto & e : vec){
            if(delay>delayCounter){
                delayCounter++;
                buffer.put(e);
            }
            else{
                auto tmp = buffer.get()*reflectionQuotient + e;
                buffer.put(tmp);
                e = tmp;
            }
        }
    }

    /*!
        \~english Set the delay of the echo in **samples**.
        \~hungarian Beállítja a visszhang késleltetését **mintákban**.
        \~english \param delay the delay in samples
        \~hungarian \param delay a kívánt késleltetés mintákban
    */
    void setDelay(const unsigned int & delay){
        this->delay = delay;
    }

    /*!
        \~english Get the delay of the echo in samples.
        \~hungarian Visszaadja a visszhang késleltetését mintákban.
        \~english \return the delay in samples
        \~hungarian \return a késleltetés mintákban
    */
    unsigned int getDelay(){
        return delay;
    }

    /*!
        \~english Set the reflection quotient which describes how much of the kinetic energy that hits a reflective surface should be reflected.
        \~hungarian Beállítja a visszaverési hányadost, ami megadja hogy a visszaverő felületet érő kinetikus energiából mennyi verődik vissza.
        \~english \param reflectionQuotient the reflection quotient
        \~hungarian \param reflectionQuotient a visszaverési hányados
    */
    void setReflectionQuotient(const float & reflectionQuotient){
        this->reflectionQuotient = reflectionQuotient;
    }

    /*!
        \~english Get the reflection quotient which describes how much of the kinetic energy that hits a reflective surface should be reflected.
        \~hungarian Visszaadja a visszaverési hányadost, ami megadja hogy a visszaverő felületet érő kinetikus energiából mennyi verődik vissza.
        \~english \return the reflection quotient 
        \~hungarian \return a visszaverési hányados
    */
    float getReflectionQuotient(){
        return reflectionQuotient;
    }

    /*!
        \~english Clear the internal state of this object, restarting the echo delay.
        \~hungarian Törli az objektum belső állapotát, ezzel újraindítva a visszhang késleltetését.
    */
    void clear(){
        buffer.clear();
        delayCounter = 0;
    }
};
};
};