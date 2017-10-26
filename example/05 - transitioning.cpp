#include <iostream>
#include <thread>
#include <chrono>

#include <SineGenerator.h>
#include <SimplePlayer.h>
#include <PortAudioBackend.h>

#define DEFAULT_CHANNELS 2
#define DEFAULT_SAMPLE_RATE 44100

int main(int argc, char * argv[]){
	if(argc<2){
		std::cout<<"No frequency given!"<<std::endl;
		return -1;
	}

    float frequency = std::atof(argv[1]);
    if(frequency<=0){
		std::cout<<"Please give a valid frequency value."<<std::endl;
		return -1;
	}

    Mixer::SineGenerator<float> sg(frequency,DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
	
	Mixer::SimplePlayer<float,Mixer::PortAudioBackend> player(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
	player.attach(sg);
    player.play();

    bool stop = false;
    
    std::thread t([&](){
        bool b = true;
        while(!stop){
            std::this_thread::sleep_for (std::chrono::seconds(2));
            if(b){
                sg.setFrequency(frequency*2,1,"ease-in-out",4);
            }
            else{
                sg.setFrequency(frequency,1);                
            }
            b=!b;
        }
    });

    std::cout<<"Alternating between a sine wave with a frequency of "<<sg.getFrequency()<<", and a sine wave of an octave higher."<<std::endl;    
	std::cout<<"Press any key to exit."<<std::endl;
    std::cin.get();
    stop = true;
    t.join();
	return 0;
}