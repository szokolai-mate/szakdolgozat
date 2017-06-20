#pragma once

#include <iostream>
#include <portaudio.h>
#include <mutex>
#include <condition_variable>


#include "iPlayer.h"
#include "iBuffer.h"
#include "utils.h"
#include "sine_generator.h"

//tmp
void Pa_error_occured(PaError err){
    Pa_Terminate();
    std::cerr<<"An error occured while using the portaudio stream"<<std::endl;
    std::cerr<<  "Error number: "<<err<<std::endl;
    std::cerr<< "Error message: "<<std::endl<<Pa_GetErrorText(err)<<std::endl;
}
//~tmp

std::mutex m;
std::condition_variable cv;

sine_generator sg(440);


//-tmp
#define PA_SAMPLE_TYPE paFloat32;

template <typename T>
class simple_player : public iPlayer<T> {
public:
    bool play();
    bool pause();
    bool stop();
	bool init(iBuffer<T> & _buffer,const audio_descriptor ad);
private:
	PaStream *stream;
	PaError err;
	//CALLBACK
	static int bufferCallback(const void *inputBuffer, void *outputBuffer,
			  unsigned long framesPerBuffer,
			  const PaStreamCallbackTimeInfo *timeInfo,
			  PaStreamCallbackFlags statusFlags,
			  void *userData)
{   
    /* Cast data passed through stream to our structure. */
    //iBuffer<float> *buffer = (iBuffer<float> *)userData;
	simple_player<float> *data = (simple_player<float> *)userData;
    float *out = (float *)outputBuffer;
    unsigned int i;
    (void)inputBuffer; /* Prevent unused variable warning. */
	//std::cout<<(data->volume)<<","<<data->buffer<<std::endl;
    /*
	float into;
	i = 0;
	bool more;
	while (i<framesPerBuffer*2) {
		more = buffer->get(into);
		if (!more) { return paContinue; }
		*out = into;
		out++;
		i++;
	}*/
	if(data->buffer->size()<framesPerBuffer*2){std::cout<<"not enough frames in buffer: "<<data->buffer->size()/2<<std::endl;}

    if(data->paused){
        data->deltaVolume=data->targetVolume-data->volume/framesPerBuffer;
    }

    float into;
    i = 0;
    unsigned int more;
    while (i < framesPerBuffer)
    {
        if(data->volume!=data->targetVolume){
            data->volume+=data->deltaVolume;
            if(data->deltaVolume>0){
                if(data->volume>data->targetVolume){
                    //fölé ment ->kész
                    data->volume=data->targetVolume;
                }
            }
            else{
                if(data->volume<data->targetVolume){
                    //alá ment->kész
                    data->volume=data->targetVolume;
                }
            }
        }
	more = data->buffer->get(into);
	if (!more)
	{
	    //return paContinue;
	}
	*out = clip::soft_clip(into*data->volume);
	out++;
	more = data->buffer->get(into);
	
	if (!more)
	{
	    //return paContinue;
	}
	*out = clip::soft_clip(into*data->volume);
	out++;
	i++;
    }
    return paContinue;
}

//SINE CALLBACK
static int sineCallback(const void *inputBuffer, void *outputBuffer,
			  unsigned long framesPerBuffer,
			  const PaStreamCallbackTimeInfo *timeInfo,
			  PaStreamCallbackFlags statusFlags,
			  void *userData)
{   
    /* Cast data passed through stream to our structure. */
	simple_player<float> *data = (simple_player<float> *)userData;
    float *out = (float *)outputBuffer;
    unsigned int i = 0;
    (void)inputBuffer; /* Prevent unused variable warning. */

    if(data->paused)std::cout<<"LAST CALLBACK"<<std::endl;
    if(data->paused){
        float distance = data->targetVolume-data->volume;
        float step = distance/framesPerBuffer;
        std::cout<<step<<"=step"<<std::endl;

        data->deltaVolume=step;
    }

    while(i<framesPerBuffer){

        if(data->volume!=data->targetVolume){
            data->volume+=data->deltaVolume;

            if(data->deltaVolume>0){
                if(data->volume>data->targetVolume){
                    //fölé ment ->kész
                    data->volume=data->targetVolume;
                }
            }
            else{
                if(data->volume<data->targetVolume){
                    //alá ment->kész
                    data->volume=data->targetVolume;
                }
            }
        }
		float sine=sg.next();
		*out=clip::soft_clip(sine*data->volume);
		out++;
		*out=clip::soft_clip(sine*data->volume);
		out++;
		i++;
	}
	return paContinue;

}


};

template <typename T>
bool simple_player<T>::pause (){
    this->targetVolume=0.0f;
    this->deltaVolume=-0.005f;
    this->paused=true;

    if(!stream){return false;}
	if(Pa_IsStreamStopped(stream)){return false;}
    err = Pa_StopStream(stream);
	if (err != paNoError){
	    Pa_error_occured(err);
        return false;
    }
    return true;
}

template <typename T>
bool simple_player<T>::play(){

    this->paused=false;
    this->targetVolume=0.5f;
    this->deltaVolume=0.05f;

    if(!stream){return false;}
    if(Pa_IsStreamActive(stream)){return false;}
    err = Pa_StartStream(stream);
    if (err != paNoError){
        Pa_error_occured(err);
        return false;
    }

    return true;
}


template <typename T>
bool simple_player<T>::stop (){
    if(!stream){return false;}
    err = Pa_StopStream(stream);
    if (err != paNoError){
        Pa_error_occured(err);
        return false;
    }
    err = Pa_CloseStream(stream);
    if (err != paNoError){
        Pa_error_occured(err);
        return false;
    }
    Pa_Terminate();

    return true;
}

template <typename T>
bool simple_player<T>::init(iBuffer<T> & _buffer,const audio_descriptor ad ){
	PaStreamParameters outputParameters;
        
    err = Pa_Initialize();
    if (err != paNoError){
		Pa_error_occured(err);
        return false;
        }
	
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice)
    {
	std::cerr<< "Error: No default output device."<<std::endl;
	if (err != paNoError){
	    Pa_error_occured(err);
        return false;
        }
    }
    outputParameters.channelCount = ad.channels;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

	this->buffer=&_buffer;
	this->volume=0.5f;
    this->targetVolume=0.0f;
    this->deltaVolume=0.0f;

	err = Pa_OpenStream(
	&stream,
	NULL, /* no input */
	&outputParameters,
	ad.sample_rate,
	256*8, //fixed for pulseaudio, testing 0 for others
	//furcsa 1024-nél VAN underflow resume+playnél, de 2048-nál NINCS!  és ez a sine-ra is igaz ahol nincs buffer!
	paClipOff,      /* we won't output out of range samples so don't bother clipping them */
	bufferCallback, /* no callback, use blocking API */
	this);		/* no callback, so no callback userData */

    if (err != paNoError){
	    Pa_error_occured(err);
        return false;
    }
	return true;
}