#pragma once

#include <utils.h>
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

const std::array<float,sine_generator::table_size> sine_generator::sinetable = constexpr_array::make<sine_generator::table_size>(sine_generator::functor);

sine_generator::sine_generator(float _frequency){
    this->step=_frequency/20;
    this->counter = 0;
}

float sine_generator::next(){
    //ez lehet a kattogás okozója
    //de nem
    //TODO: bepakolni valami ismétlódő bufferbe és ugy kinyomni? hátha akkor nem kattog
    //??? szerintem ezek a kattogások a bufferelés miatt vannak
    //counter=(int)(counter+step) % SINE_TABLE_SIZE;
    counter+=step;
    if(counter>=SINE_TABLE_SIZE){counter-=SINE_TABLE_SIZE;}
    //if((int)counter!=(int)(counter-step)){std::cout<<"->"<<counter<<std::endl;}
    return sinetable[counter];
}

float sine_generator::getFrequency(){
    return this->step*20.0f;
}

//test
void sine_generator::setFrequency(float _frequency){
    this->step=_frequency/20;
}