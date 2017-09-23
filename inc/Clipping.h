#pragma once

#include <math.h>

namespace Clipping
{

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