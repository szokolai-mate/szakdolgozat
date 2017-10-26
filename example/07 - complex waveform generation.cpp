/*! 
*	Example 7 - complex waveform generator
*
*	Demonstrates spectral synthesis using a complex waveform generator.
*/
#include <iostream>

#include <SimplePlayer.h>
#include <PortAudioBackend.h>
#include <QueueBuffer.h>
#include <ComplexWaveformGenerator.h>

#define DEFAULT_CHANNELS 2
#define DEFAULT_SAMPLE_RATE 44100

int main(int argc, char * argv[]){
    DataFlow::QueueBuffer<float> buffer;

    /* we define frequency and amplitude pairs, thus defining the signal's power spectrum */
    std::vector<std::pair<float,float>> components;
    /* the first harmonic */
    components.push_back(std::make_pair(220,1));
    /* some of the harmonic's overtones */
    components.push_back(std::make_pair(440,0.6f));
    components.push_back(std::make_pair(660,0.4f));    
    components.push_back(std::make_pair(880,0.1f));
    /* some random inharmonics */
    components.push_back(std::make_pair(599,0.1f));
    components.push_back(std::make_pair(294,0.01f));
    components.push_back(std::make_pair(954,0.01f));
    components.push_back(std::make_pair(744,0.05f));
    components.push_back(std::make_pair(489,0.08f));
    /* and some not so random inharmonics between 660Hz and 880Hz*/
    int fineness = 20;
    for(int i = 1 ;i<=fineness;i++){
        float frequency = 660+(i*219/(float)fineness);
        float amplitude = 0.03f/i;
        components.push_back(std::make_pair(frequency,amplitude));
    }
    /* give our spectral components to a new generator */
    Mixer::ComplexWaveformGenerator<float> gen(components,DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);

    /* since this is some pretty heavy calculation, we need to buffer it first */
    float seconds = 5;
    int samples = DEFAULT_CHANNELS * DEFAULT_SAMPLE_RATE * seconds;
    while(buffer.size()<samples-512){
        buffer.put(gen.get(512));
    }
    buffer.put(gen.get(samples-buffer.size()));

    /* lower the volume, since the signal will definitely be above treshold */
    DataFlow::Applicator<float,VolumeControl> vc;
	vc.getMethod().setVolume(0.5f);
    vc.attach(buffer);

    Mixer::SimplePlayer<float,Mixer::PortAudioBackend> player(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
	player.attach(vc);
	player.play();

    std::cout<<"Playing "<<seconds<<" seconds of the synthesized waveform."<<std::endl;
    std::cout<<"Press any key to exit."<<std::endl;
	std::cin.get();
	return 0;
}