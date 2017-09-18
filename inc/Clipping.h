#pragma once

#include <math.h>

namespace Clipping
{

class soft_clip
{
    inline float operator()(float x)
    {
        if (x > 0.8f || x < -0.8f)
        {
            return ((pow(exp(1), (exp(1) * x)) - 1) / (pow(exp(1), (exp(1) * x)) + 1));
        }
        else
            return x;
    }
};
};