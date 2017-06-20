#ifndef SIMPLE_PLAYER_HEADER
#define SIMPLE_PLAYER_HEADER

#include <iostream>
#include <portaudio.h>
#include <mutex>
#include <condition_variable>


#include <iPlayer.h>
#include <iBuffer.h>
#include <Utils.h>
#include <SineGenerator.h>

/*Design flaw:
    Portaudio callbacks are static, ergo they can only play what they originally receive.
    best to only give one single object the fully mixed stream at the end and players will be sort of like steams, while managers are stream-mixers
    */

//TODO what the hell is going on with this code


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

    //tmp
    //sine_generator sg(440);
    //int counter = 0;
    //bool change = false;
    //~tmp

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
        //tmp
        //removed cuz of static
        /*
		float sine=sg.next();
		++counter;
		if(counter>48000*3){
			counter=0;
			if(change){sg.setFrequency(880);change=false;}
			else{change = true; sg.setFrequency(440);}
		}
        */
        float sine = 1.0f;
		//~tmp
		*out=clip::soft_clip(sine*data->volume);
		out++;
		*out=clip::soft_clip(sine*data->volume);
		out++;
		i++;
	}
	return paContinue;

}


};
#endif