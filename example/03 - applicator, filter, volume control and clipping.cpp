/*! 
*	Example 3 - using filters with applicators
*
*	This example program will demonstrate the usage of the applicator.
*/
#include <iostream>

#include <OggFileLoader.h>
#include <VorbisDecoder.h>
#include <SimplePlayer.h>
#include <PortAudioBackend.h>
#include <Applicator.h>
#include <Filters.h>
#include <VolumeControl.h>
#include <Clipping.h>

#define DEFAULT_CHANNELS 2
#define DEFAULT_SAMPLE_RATE 44100

int main(int argc, char * argv[]){
	/* Checking argument */
	/***********************************************************/
    if(argc<2){
		std::cout<<"No filename given!"<<std::endl;
		return -1;
	}
	/***********************************************************/

	std::string fname{argv[1]};

	OggFileLoader<float,VorbisDecoder> loader;
	/* open file */
	if(!loader.open(fname)){
		std::cout<<"File not found!"<<std::endl;
		return -1;
	}
	/* initialize the decoder */
	loader.init();

	/* Declare an applicator. This one will apply an echo effect to the attached source. */
	DataFlow::Applicator<float,Mixer::Filter::Echo> filter;
	/* set the echo filter's parameters */
	filter.getMethod().setDelay(DEFAULT_CHANNELS*DEFAULT_SAMPLE_RATE*0.5f);
	filter.getMethod().setReflectionQuotient(0.5f);
	/* attach the loader as the filter's source */
	filter.attach(loader);

	/* Another applicator, which will act as volume control. */
	DataFlow::Applicator<float,VolumeControl> vc;
	/* set the volume */
	vc.getMethod().setVolume(0.3f);
	/* attach the filter as the volume control's source */
    vc.attach(filter);
	
	/* The last applicator, which will apply hard clipping. */
	DataFlow::Applicator<float,Clipping::Hard> clipping;
	/* attach the volume control as the clipper's source */
	clipping.attach(vc);
	
	Mixer::SimplePlayer<float,Mixer::PortAudioBackend> player(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
	player.attach(clipping);
	/* And finally, attach the clipper as the player's source, finishing the source chain. */
	player.play();

	std::cout<<"Press any key to exit."<<std::endl;
	std::cin.get();
	return 0;
}