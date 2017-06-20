#include <SimplePlayer.h>
//tmp
void Pa_error_occured(PaError err){
    Pa_Terminate();
    std::cerr<<"An error occured while using the portaudio stream"<<std::endl;
    std::cerr<<  "Error number: "<<err<<std::endl;
    std::cerr<< "Error message: "<<std::endl<<Pa_GetErrorText(err)<<std::endl;
}
//~tmp

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