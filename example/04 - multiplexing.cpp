/*! 
*	Example 4 - multiplexing
*
*	Demonstrates multiplexing - combining single-channel sources into a multichannel source.
*/
#include <iostream>

#include <SineGenerator.h>
#include <SimplePlayer.h>
#include <PortAudioBackend.h>
#include <Multiplexer.h>
#include <CheapSawtoothGenerator.h>

#define DEFAULT_SAMPLE_RATE 44100

int main(int argc, char * argv[]){
	/* Checking argument */
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

	/* make two mono sawtooth generators, one with the set frequency and the other an octave higher */
    Mixer::CheapSawtoothGenerator<float> sg(frequency,1,DEFAULT_SAMPLE_RATE);
    Mixer::CheapSawtoothGenerator<float> sg2(frequency*2,1,DEFAULT_SAMPLE_RATE);
	
	/* make the multiplexer which will supply 2 channeled data */
	Mixer::Multiplexer<float> mx(2);
	/* set the channel sources */
    mx.setChannel(1,sg);
    mx.setChannel(2,sg2);
    
	Mixer::SimplePlayer<float,Mixer::PortAudioBackend> player(2,DEFAULT_SAMPLE_RATE);
	player.attach(mx);
	player.play();

    std::cout<<"Playing a sawtooth wave with a frequency of "<<sg.getFrequency()<<" on channel 1, and a sawtooth wave an octave higher on channel 2."<<std::endl;    
	std::cout<<"Press any key to exit."<<std::endl;
	std::cin.get();
	return 0;
}