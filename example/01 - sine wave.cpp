/*! 
*	Example 1 - sine wave
*
*	This example program will play a sine wave with the given frequency.
*/

#include <iostream>

#include <SineGenerator.h>
#include <SimplePlayer.h>
#include <PortAudioBackend.h>

#define DEFAULT_CHANNELS 2
#define DEFAULT_SAMPLE_RATE 44100

int main(int argc, char * argv[]){
	/* Checking arguments */
	/***********************************************************/
	if(argc<2){
		std::cout<<"No frequency given!"<<std::endl;
		return -1;
	}

    float frequency = std::atof(argv[1]);
    if(frequency<=0){
		std::cout<<"Please give a valid frequency value."<<std::endl;
		return -1;
	}
	/***********************************************************/

    Mixer::SineGenerator<float> sg(frequency,DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
	
	Mixer::SimplePlayer<float,Mixer::PortAudioBackend> player(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
	/* attach the sine generator as the source */
	player.attach(sg);
	player.play();

    std::cout<<"Playing a sine wave with a frequency of "<<sg.getFrequency()<<"."<<std::endl;    
	std::cout<<"Press any key to exit."<<std::endl;
	std::cin.get();
	return 0;
}