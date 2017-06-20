#pragma once

#include <Utils.h>
#include <iostream>
//TODO: változtatható frekvencia
//TODO: szétszedni h+cpp
#define SINE_TABLE_SIZE 2400
class sine_generator{
private:
    int counter;
    float step;

    static float functor(double x) { return (float)sin(((double)x / (double)sine_generator::table_size) * M_PI * 2.0); }

public:
    constexpr static const int table_size = SINE_TABLE_SIZE;
    static const std::array<float,table_size> sinetable;

    sine_generator(float _frequency);
    float next();
    float getFrequency();

    //test
    void setFrequency(float _frequency);
};