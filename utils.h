#pragma once

#include <math.h>

namespace clip{

    inline float soft_clip(float x) {
        //az ellenőrzést lehet hogy kijjebb kellene tenni
        if (x > 0.8f || x < -0.8f) {
            return ( (pow(exp(1),(exp(1)*x)) - 1) / (pow(exp(1), (exp(1)*x)) + 1) );
        }
        else return x;
    }

}


//structs

struct audio_descriptor {
	unsigned short channels;
	unsigned short sample_rate;

	audio_descriptor() {}
	audio_descriptor(unsigned short _channels, unsigned short _sample_rate) {
		channels = _channels;
		sample_rate = _sample_rate;
	}
};

struct audio_file_descriptor {
    unsigned long data_start;
    unsigned long next_byte;

    audio_file_descriptor(){}
    audio_file_descriptor(unsigned long _data_start,unsigned long _next_byte){
        data_start=_data_start;
        next_byte=_next_byte;
    }
    audio_file_descriptor(unsigned long _data_start){
        data_start=_data_start;
        next_byte=_data_start;
    }

};