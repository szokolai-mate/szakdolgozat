/*! 
*	Example 6 - mixing with consolidator
*
*	Demonstrates mixing two audio sources by using a consolidator, which will convert multiple sources into one by adding their elements together.
*/
#include <iostream>

#include <OggFileLoader.h>
#include <VorbisDecoder.h>
#include <SimplePlayer.h>
#include <PortAudioBackend.h>
#include <Consolidator.h>
#include <ConsolidationMethods.h>
#include <SineGenerator.h>

#define DEFAULT_CHANNELS 2
#define DEFAULT_SAMPLE_RATE 44100

int main(int argc, char * argv[]){
	/* Checking argument */
	/***********************************************************/
	if(argc<3){
		std::cout<<"Please give a filename and a frequency!"<<std::endl;
		return -1;
    }

    float frequency = std::atof(argv[2]);
    if(frequency<=0){
		std::cout<<"Please give a valid frequency value."<<std::endl;
		return -1;
	}
	/***********************************************************/

	std::string fname{argv[1]};

	OggFileLoader<float,VorbisDecoder> loader;
	if(!loader.open(fname)){
		std::cout<<"File not found!"<<std::endl;
		return -1;
	}
    loader.init();
    
    Mixer::SineGenerator<float> sg(frequency,DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
	
	/* make the consolidator with the Accumulation method */
	DataFlow::Consolidator<float,Consolidation::Accumulation> con;
	/* attach both sources to the consolidator */
    con.attach(loader);
    con.attach(sg);
	
	Mixer::SimplePlayer<float,Mixer::PortAudioBackend> player(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
	player.attach(con);
	player.play();

	std::cout<<"Press any key to exit."<<std::endl;
	std::cin.get();
	return 0;
}