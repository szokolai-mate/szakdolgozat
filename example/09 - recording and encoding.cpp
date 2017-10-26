#include <iostream>

#include <SimplePlayer.h>
#include <PortAudioBackend.h>
#include <Recorder.h>
#include <QueueBuffer.h>
#include <VorbisEncoder.h>
#include <OggFileLoader.h>
#include <VorbisDecoder.h>

#define DEFAULT_CHANNELS 2
#define DEFAULT_SAMPLE_RATE 44100

int main(int argc, char * argv[]){
    if(argc<3){
		std::cout<<"Please give a filename and length in seconds!"<<std::endl;
		return -1;
    }

    float seconds = std::atof(argv[2]);
    if(seconds<=0){
		std::cout<<"Please give a valid length in seconds."<<std::endl;
		return -1;
	}

    std::string fname{argv[1]};
    
    DataFlow::QueueBuffer<float> buffer;
    Mixer::Recorder<float> rec(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
    rec.attach(buffer);
    std::cout<<"Recording for "<<seconds<<" seconds."<<std::endl;    
    rec.record(seconds);

    VorbisEncoder encoder(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE,1);
    if(!encoder.open(fname)){
        std::cout<<"Could not open file!"<<std::endl;
		return -1;
    }
    encoder.addComment("test","comment");
    encoder.initEncoding();
    while(buffer.size()>512){
        encoder.add(buffer.get(512));
    }
    encoder.add(buffer.get(buffer.size()));
    encoder.close();
    
    OggFileLoader<float,VorbisDecoder> loader;
	if(!loader.open(fname)){
		std::cout<<"Generated file not found!"<<std::endl;
		return -1;
	}
    loader.init();

    Mixer::SimplePlayer<float,Mixer::PortAudioBackend> player(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
	player.attach(loader);
	player.play();

	std::cout<<"Playing back the recorded audio. Press any key to exit."<<std::endl;
	std::cin.get();
	return 0;
}