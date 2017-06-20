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
int counter = 0;
bool change = false;


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
	//ez miért nem segít?
	if(data->buffer->size()<framesPerBuffer*2){std::cout<<"not enough frames in buffer: "<<data->buffer->size()/2<<std::endl;return paContinue;}
    float into;
    i = 0;
    unsigned int more;
    while (i < framesPerBuffer)
    {
	more = data->buffer->get(into);
	if (!more)
	{
	    return paContinue;
	}
	*out = clip::soft_clip(into*data->volume);
	out++;
	more = data->buffer->get(into);
	
	if (!more)
	{
	    return paContinue;
	}
	*out = clip::soft_clip(into*data->volume);
	out++;
	i++;
	//volume increase of noise
	//data->volume += 0.00001f;
	if (data->volume > 2.0f)
	{
	    data->volume = 0.0f;
	}
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

	while(i<framesPerBuffer){
		float sine=sg.next();
		//tmp
		++counter;
		if(counter>48000*3){
			counter=0;
			if(change){sg.setFrequency(880);change=false;}
			else{change = true; sg.setFrequency(440);}
		}

		//~tmp
		*out=clip::soft_clip(sine*data->volume);
		out++;
		*out=clip::soft_clip(sine*data->volume);
		out++;
		i++;
	}
	std::cout<<data->volume<<std::endl;
	return paContinue;

}

};


template <typename T>
bool simple_player<T>::pause (){
	if(!stream){return false;}
	if(Pa_IsStreamStopped(stream)){return false;}
	//wait for buffer.ready?
	//test
	while(this->volume>0.0f){
		this->volume-=0.01f;
		std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 1000>>(1));
		//std::cout<<this->volume<<std::endl;
	}
    err = Pa_StopStream(stream);
	if (err != paNoError){
	    Pa_error_occured(err);
        return false;
    }
	return true;
}

template <typename T>
bool simple_player<T>::stop (){
	if(!stream){return false;}
	//test
	while(this->volume>0.0f){
		this->volume-=0.001f;
		std::this_thread::yield();
		std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 1000>>(1));
		//std::cout<<this->volume<<std::endl;

	}

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
bool simple_player<T>::play(){
	if(!stream){return false;}
	if(Pa_IsStreamActive(stream)){return false;}
	err = Pa_StartStream(stream);
	if (err != paNoError){
	    Pa_error_occured(err);
        return false;
    }
	//test
	while(this->volume<0.5f){
		this->volume+=0.01f;
		std::this_thread::yield();
		std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 1000>>(1));
		//std::cout<<this->volume<<std::endl;

	}
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

	err = Pa_OpenStream(
	&stream,
	NULL, /* no input */
	&outputParameters,
	ad.sample_rate,
	256*8, //fixed for pulseaudio, testing 0 for others
	//furcsa 1024-nél VAN underflow resume+playnél, de 2048-nál NINCS!  és ez a sine-ra is igaz ahol nincs buffer!
	paClipOff,      /* we won't output out of range samples so don't bother clipping them */
	sineCallback, /* no callback, use blocking API */
	this);		/* no callback, so no callback userData */

    if (err != paNoError){
	    Pa_error_occured(err);
        return false;
    }
	return true;
}