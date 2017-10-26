/*! 
*	Example 9 - recording and encoding
*
*	Records for the given time from the default input device, then encodes the audio into a file.
*/
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
    /* Checking arguments */
	/***********************************************************/
    if(argc<3){
		std::cout<<"Please give a filename and length in seconds!"<<std::endl;
		return -1;
    }

    float seconds = std::atof(argv[2]);
    if(seconds<=0){
		std::cout<<"Please give a valid length in seconds."<<std::endl;
		return -1;
	}
	/***********************************************************/

    std::string fname{argv[1]};
    
    DataFlow::QueueBuffer<float> buffer;
    Mixer::Recorder<float> rec(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
    /* attach a buffer to the recorder, so that the recorded audio will be put into this buffer */
    rec.attach(buffer);
    std::cout<<"Recording for "<<seconds<<" seconds."<<std::endl;
    /* records for the set time */
    rec.record(seconds);

    /* Declare the encoder. The last argument is the encoded audio quality, 1 being the highest and 0.1 the lowest. */
    VorbisEncoder encoder(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE,1);
    if(!encoder.open(fname)){
        std::cout<<"Could not open file!"<<std::endl;
		return -1;
    }
    /* Vorbis supports adding user comments, which are key-value pairs. Add a test comment. */
    encoder.addComment("test","comment");
    /* Initialize the encoding which locks the header down. We can't add comments after this. */
    encoder.initEncoding();
    /* empty the buffer of recorded data into the encoder */
    while(buffer.size()>512){
        encoder.add(buffer.get(512));
    }
    encoder.add(buffer.get(buffer.size()));
    /* finalize the encoding and close the file */
    encoder.close();
    
    /* then play it back */
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