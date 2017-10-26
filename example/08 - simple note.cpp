#include <iostream>

#include <SimplePlayer.h>
#include <PortAudioBackend.h>
#include <RepeatingBuffer.h>
#include <ComplexWaveformGenerator.h>
#include <SimpleNote.h>

#define DEFAULT_CHANNELS 2
#define DEFAULT_SAMPLE_RATE 44100

int main(int argc, char * argv[]){
    DataFlow::RepeatingBuffer<float> repeating;

    float fundamentalFrequency = 90;
    std::vector<std::pair<float,float>> components;
    components.push_back(std::make_pair(fundamentalFrequency,1));    
    components.push_back(std::make_pair(fundamentalFrequency*2,0.4f));
    components.push_back(std::make_pair(fundamentalFrequency*3,0.3f));
    components.push_back(std::make_pair(fundamentalFrequency*4,0.2f));    
    components.push_back(std::make_pair(fundamentalFrequency*5,0.1f));
    components.push_back(std::make_pair(355,0.1f));
    components.push_back(std::make_pair(147,0.01f));
    components.push_back(std::make_pair(287,0.01f));
    components.push_back(std::make_pair(204,0.05f));
    components.push_back(std::make_pair(114,0.08f));
    int fineness = 100;
    for(int i = 1 ;i<=fineness;i++){
        float frequency = fundamentalFrequency+(i*fundamentalFrequency*5/(float)fineness);
        float amplitude = 0.2f/i;
        components.push_back(std::make_pair(frequency,amplitude));
    }
    Mixer::ComplexWaveformGenerator<float> gen(components,DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);

    Mixer::SimpleNote<float,Transition::EaseIn::Quadratic,Transition::EaseOut::Quintic> note(0.4f,DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
    note.attach(gen);
    note.setAttack(0.15f);
    note.setDecay(0.3f);
    note.bakeNote();

    auto noteData = note.get(note.size());
    float pauseInSeconds = 0.4f;
    std::vector<float> silence = std::vector<float>(pauseInSeconds*DEFAULT_CHANNELS*DEFAULT_SAMPLE_RATE,0);
    auto combinedData = silence;
    for(auto e : noteData){
        combinedData.push_back(e);
    }

    repeating.put(combinedData);

    DataFlow::Applicator<float,VolumeControl> vc;
	vc.getMethod().setVolume(0.5f);
    vc.attach(repeating);

    Mixer::SimplePlayer<float,Mixer::PortAudioBackend> player(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
	player.attach(vc);
	player.play();

    std::cout<<"Playing the generated note on repeat."<<std::endl;
    std::cout<<"Press any key to exit."<<std::endl;
	std::cin.get();
	return 0;
}