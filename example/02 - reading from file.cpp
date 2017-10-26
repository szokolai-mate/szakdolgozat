/*! 
*	Example 2 - reading from a file
*
*	This example program will open and play an OGG file.
*/
#include <iostream>
#include <OggFileLoader.h>
#include <VorbisDecoder.h>
#include <SimplePlayer.h>
#include <PortAudioBackend.h>

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
	/* initialize the decoding */
	loader.init();
	
	Mixer::SimplePlayer<float,Mixer::PortAudioBackend> player(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
	/* attach the loader as the source */
	player.attach(loader);
	player.play();

	std::cout<<"Press any key to exit."<<std::endl;
	std::cin.get();
	return 0;
}