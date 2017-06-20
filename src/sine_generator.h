#include <SineGenerator.h>

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