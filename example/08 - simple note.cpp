/*! 
*	Example 8 - simple note
*
*	Demonstrates making simple notes. We set a complex waveform as the note's base and set an amplitude envelope to describe it's volume to time function.
*/
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

    /* make a pleasing audio waveform with spectral synthesis */
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

    /* declare the note, we set the volume increase's function, the volume decrease's function and length of the note in seconds */
    /* for example this will be a 0.4 seconds long note, the volume increase will be an ease-in curve and the decrease an ease-out one */
    Mixer::SimpleNote<float,Transition::EaseIn::Quadratic,Transition::EaseOut::Quintic> note(0.4f,DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
    /* set the complex wave as the base */
    note.attach(gen);
    /* set the end of the attack (where volume increases) to 15% of the note, 0.06 seconds in this case */
    note.setAttack(0.15f);
    /* set the start of the decay (where volume decreases) to 30% of the note, 0.12 seconds in this case */
    note.setDecay(0.3f);
    /* "bake" the note, calculating the audio data and placing it into an internal buffer */
    note.bakeNote();

    /* copy the whole note into a vector */
    auto noteData = note.get(note.size());
    /* make another vector of silence */
    float pauseInSeconds = 0.4f;
    std::vector<float> silence = std::vector<float>(pauseInSeconds*DEFAULT_CHANNELS*DEFAULT_SAMPLE_RATE,0);
    auto combinedData = silence;
    /* put our note at the end of the silence */
    for(auto e : noteData){
        combinedData.push_back(e);
    }

    /* fill a repeating buffer with this "silence then our note" vector */
    repeating.put(combinedData);

    /* lower the volume a bit */
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