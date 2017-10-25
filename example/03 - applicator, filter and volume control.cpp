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
    if(argc<2){
		std::cout<<"No filename given!"<<std::endl;
		return -1;
	}

	std::string fname{argv[1]};

	OggFileLoader<float,VorbisDecoder> loader;
	if(!loader.open(fname)){
		std::cout<<"File not found!"<<std::endl;
		return -1;
	}
	loader.init();

	DataFlow::Applicator<float,Mixer::Filter::Echo> filter;
	filter.getMethod().setDelay(DEFAULT_CHANNELS*DEFAULT_SAMPLE_RATE*0.5f);
	filter.getMethod().setReflectionQuotient(0.5f);
	filter.attach(loader);

	DataFlow::Applicator<float,VolumeControl> vc;
	vc.getMethod().setVolume(0.3f);
    vc.attach(filter);
    
    DataFlow::Applicator<float,Clipping::Hard> clipping;
	clipping.attach(vc);
	
	Mixer::SimplePlayer<float,Mixer::PortAudioBackend> player(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
	player.attach(clipping);
	player.play();

	std::cout<<"Press any key to exit."<<std::endl;
	std::cin.get();
	return 0;
}