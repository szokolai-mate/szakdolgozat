#pragma once

#include <math.h>
#include <vector>


//! \~english Contains classes implementing clipping methods.
//! \~hungarian Nyírási módókat megvalósító osztályokat tartalmaz.
/*! \~english Must implement them for singe values and vectors of values with arbitary length.
    \~hungarian Két implementáció szükséges: egyedüli értékekhez és bármilyen hosszúságú vektorokhoz.
*/
namespace Clipping
{
//! \~english Soft clipping implementation. Values above or below 80% of the limits get reduced to never reach the limits. Similar to hyperbolic tangent function.
//! \~hungarian Finom nyírás. A határértékek 80%-át meghaladó értékeket úgy csökkenti hogy soha ne érjék el a határértékeket. Hiperbólikus tangens függvényhez hasonló.
class Soft
{
  public:
    inline float operator()(float x)
    {
        if (x > 0.8f || x < -0.8f)
        {
            return ((pow(exp(1), (exp(1) * x)) - 1) / (pow(exp(1), (exp(1) * x)) + 1));
        }
        else
            return x;
    }

    inline void operator()(std::vector<float> &vector)
    {
        for (float &x : vector)
        {
            if (x > 0.8f || x < -0.8f)
            {
                x = ((pow(exp(1), (exp(1) * x)) - 1) / (pow(exp(1), (exp(1) * x)) + 1));
            }
        }
    }
};

//! \~english Hard clipping implementation. Values above or below the limits get clipped to the limits.
//! \~hungarian Durva nyírás. A határértékeken túli értékek a határértéket veszik fel.
class Hard
{
public:
    inline float operator()(float x)
    {
        if (x > 1.0f)
        {
            return 1.0f;
        }
        else
        {
            if (x < -1.0f)
            {
                return -1.0f;
            }
        }
        return x;
    }

    inline void operator()(std::vector<float> &vector)
    {
        for (float &x : vector)
        {
            if (x > 1.0f)
            {
                x = 1.0f;
            }
            else
            {
                if (x < -1.0f)
                {
                    x = -1.0f;
                }
            }
        }
    }
};
};