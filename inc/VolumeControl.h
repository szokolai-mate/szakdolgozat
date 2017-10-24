#pragma once

#include <vector>
#include <memory>

#include <iSource.h>
#include <Transitioner.h>
#include <TransitionFunctions.h>
#include <iTransitioner.h>

template <typename T>
/*! \~english A functor that acts as volume control when used with Applicator.
    \~hungarian Egy funktor ami hangerőszabályzóként viselkedik egy Applicator -ban.
*/
class VolumeControl
{
  private:
    float volume;
    DataFlow::iSource<T> *source;
    std::unique_ptr<iTransitioner<T>>  transitioner;

    void stepTransition(const int & amount){
        if(transitioner){
            transitioner->step(amount);
        }
    }

  public:
    /*! \~english Get the current volume multiplier.
        \~hungarian Visszaadja a jelenlegi hangerő szorzót.
        \~english \return the volume multiplier
                \~hungarian \return a hangerő szorzó
    */
    T getVolume()
    {
        return volume;
    }

    /*! \~english Set the volume multiplier.
        \~hungarian Beállítja a hangerő szorzót.
        \~english \param volume the desired volume
                        \~hungarian \param volume a kívánt hangerő
    */
    void setVolume(const T &volume)
    {
        this->volume = volume;
    }

    inline T operator()(T x)
    {
        stepTransition(1);
        return x * volume;
    }

    inline void operator()(std::vector<T> &vector)
    {
        for (T &x : vector)
        {
            x = x * volume;
            stepTransition(1);      
        }
    }

    /*!
      \~english Set the volume with the desired transition.
      \~hungarian Beállítja a hangerőt a kívánt átmenettel.
  
      \~english \param volume the desired volume
      \~hungarian \param volume a kívánt hangerő
      
      \~english \param seconds the length of transition in seconds
      \~hungarian \param seconds az átmenet hossza másodpercben
      
      \~english \param valuesPerSecond the number of samples that will go through this object per second
      \~hungarian \param valuesPerSecond az objektumon átmenő értékek száma másodpercenként
      
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
    void setVolume(const float & newVolume, const float &seconds, const int & valuesPerSecond, std::string method = "linear", const unsigned int &degree = 2){
            int samples = valuesPerSecond * seconds;
            if ("linear" == method)
            {
                this->transitioner = std::make_unique<Transitioner<T, Transition::Linear>>(this->volume);
            }
            switch (degree)
            {
            case 2:
                if ("ease-in-out" == method)
                {
                    this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Quadratic>>(this->volume);
                    break;
                }
                if ("ease-in" == method)
                {
                    this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Quadratic>>(this->volume);
                    break;
                }
                if ("ease-out" == method)
                {
                    this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Quadratic>>(this->volume);
                    break;
                }
        
            case 3:
                if ("ease-in-out" == method)
                {
                    this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Cubic>>(this->volume);
                    break;
                }
                if ("ease-in" == method)
                {
                    this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Cubic>>(this->volume);
                    break;
                }
                if ("ease-out" == method)
                {
                    this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Cubic>>(this->volume);
                    break;
                }
        
            case 4:
                if ("ease-in-out" == method)
                {
                    this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Quartic>>(this->volume);
                    break;
                }
                if ("ease-in" == method)
                {
                    this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Quartic>>(this->volume);
                    break;
                }
                if ("ease-out" == method)
                {
                    this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Quartic>>(this->volume);
                    break;
                }
        
            case 5:
                    if ("ease-in-out" == method)
                    {
                        this->transitioner = std::make_unique<Transitioner<T, Transition::EaseInOut::Quintic>>(this->volume);
                        break;
                    }
                    if ("ease-in" == method)
                    {
                        this->transitioner = std::make_unique<Transitioner<T, Transition::EaseIn::Quintic>>(this->volume);
                        break;
                    }
                    if ("ease-out" == method)
                    {
                        this->transitioner = std::make_unique<Transitioner<T, Transition::EaseOut::Quintic>>(this->volume);
                        break;
                    }
            default:
                this->transitioner = std::make_unique<Transitioner<T, Transition::Linear>>(this->volume);
            }
        
            this->transitioner->init(this->getVolume(), newVolume, samples);
        }

    VolumeControl(): volume(1){}
    /*!
        \~english Constructor. Sets volume.
        \~hungarian Konstruktor. Beállítja a hangerőt.
        \~english \param volume the desired volume
        \~hungarian \param volume a kívánt hangerő
    */
    VolumeControl(const T & volume): volume(volume){}
};