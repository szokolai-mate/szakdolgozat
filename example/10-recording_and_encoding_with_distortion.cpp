/*! 
*	Example 9 - recording and encoding
*
*	Records for the given time from the default input device, then encodes the audio into a file.
*/
#include <iostream>
#include <thread>
#include <chrono>

#include <SimplePlayer.h>
#include <PortAudioBackend.h>
#include <Recorder.h>
#include <QueueBuffer.h>
#include <VorbisEncoder.h>
#include <OggFileLoader.h>
#include <VorbisDecoder.h>
#include <Applicator.h>
#include <Filters.h>
#include <Clipping.h>

#define DEFAULT_CHANNELS 2
#define DEFAULT_SAMPLE_RATE 44100

int main(int argc, char *argv[])
{
    /* Checking arguments */
    /***********************************************************/
    if (argc < 3)
    {
        std::cout << "Please give a filename, length in seconds and the degree of distortion!" << std::endl;
        return -1;
    }

    float seconds = std::atof(argv[2]);
    if (seconds <= 0)
    {
        std::cout << "Please give a valid length in seconds." << std::endl;
        return -1;
    }

    int degree = std::atoi(argv[3]);
    if (degree <= 0)
    {
        std::cout << "Please give a valid degree of distortion." << std::endl;
        return -1;
    }
    /***********************************************************/

    std::string fname{argv[1]};

    DataFlow::QueueBuffer<float> buffer;
    Mixer::Recorder<float> rec(DEFAULT_CHANNELS, DEFAULT_SAMPLE_RATE);
    /* attach a buffer to the recorder, so that the recorded audio will be put into this buffer */
    rec.attach(buffer);
    std::cout << "Recording for " << seconds << " seconds." << std::endl;
    /* timer */
    std::cout << "3..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "2..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "1..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    /* records for the set time */
    rec.record(seconds);

    /* set up the distortion applicator */
    DataFlow::Applicator<float,Mixer::Filter::Fuzzbox> filter;
    filter.getMethod().setDegree(degree);
    filter.attach(buffer);
    /* and the clipping */
    DataFlow::Applicator<float,Clipping::Hard> clipping;
    clipping.attach(filter);
    

    /* Declare the encoder. The last argument is the encoded audio quality, 1 being the highest and 0.1 the lowest. */
    VorbisEncoder encoder(DEFAULT_CHANNELS, DEFAULT_SAMPLE_RATE, 1);
    if (!encoder.open(fname))
    {
        std::cout << "Could not open file!" << std::endl;
        return -1;
    }
    /* Vorbis supports adding user comments, which are key-value pairs. Add a test comment. */
    encoder.addComment("test", "comment");
    /* Initialize the encoding which locks the header down. We can't add comments after this. */
    encoder.initEncoding();
    /* empty the buffer of recorded data into the encoder */
    std::vector<float> vec = clipping.get(512);
    while (vec.size() > 0)
    {
        encoder.add(vec);
        vec = clipping.get(512);
    }
    /* finalize the encoding and close the file */
    encoder.close();

    /* then play it back */
    OggFileLoader<float, VorbisDecoder> loader;
    if (!loader.open(fname))
    {
        std::cout << "Generated file not found!" << std::endl;
        return -1;
    }
    loader.init();

    Mixer::SimplePlayer<float, Mixer::PortAudioBackend> player(DEFAULT_CHANNELS, DEFAULT_SAMPLE_RATE);
    player.attach(loader);
    player.play();

    std::cout << "Playing back the distorted audio. Press any key to exit." << std::endl;
    std::cin.get();
    player.stop();
    return 0;
}