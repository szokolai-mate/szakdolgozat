#pragma once

#include <vector>

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
};
};