//!\todo TODO(later): equalizer

//probléma: pause-nál eldobja a mostani get-et

//!\todo TODO:blocking portaudio
//!\todo TODO:microphone
//!\todo TODO:modulating filter

#include <iostream>
#include <thread>
#include <chrono>

#include <Utils.h>

#include <OggFileLoader.h>
#include <SimplePlayer.h>
#include <PortAudioBackend.h>
#include <SineGenerator.h>
#include <SawtoothGenerator.h>
#include <Consolidator.h>
#include <ConsolidationMethods.h>
#include <Applicator.h>
#include <Clipping.h>
#include <VolumeControl.h>

#include <QueueBuffer.h>
#include <VorbisDecoder.h>

#define DEFAULT_CHANNELS 2
#define DEFAULT_SAMPLE_RATE 44100

#define DEBUG_PORTAUDIO
#ifdef DEBUG_PORTAUDIO
#include <portaudio.h>
#endif

int main()
{
#ifdef DEBUG_PORTAUDIO
	PaError err;
	err = Pa_Initialize();
	if (err != paNoError)
	{
		std::cout << "PortAudio error: " << err << std::endl
				  << Pa_GetErrorText(err) << std::endl;
	}
//#define DEBUG_DEVICES
#ifdef DEBUG_DEVICES
	std::cout << Pa_GetVersionText() << std::endl;
	int n = Pa_GetDeviceCount();
	std::cout << n << " device(s) found." << std::endl;
	for (int i = 0; i < n; i++)
	{
		PaDeviceInfo const *info = Pa_GetDeviceInfo(i);
		if (!info)
		{
			std::cout << "device not found" << std::endl;
		}
		else
		{
			PaHostApiInfo const *apiinfo = Pa_GetHostApiInfo(info->hostApi);
			printf("%s of API %s\n", info->name, apiinfo->name);
		}
	}
#endif

//#define DEBUG_APIS
#ifdef DEBUG_APIS
	int hosts = Pa_GetHostApiCount();
	std::cout << n << " host(s) found." << std::endl;
	for (int i = 0; i < hosts; i++)
	{
		PaHostApiInfo const *info = Pa_GetHostApiInfo(i);
		if (!info)
		{
			std::cout << "host not found" << std::endl;
		}
		else
		{
			printf("%d:%s ,type %d\n", i, info->name, info->type);
			printf("%d devices\n", info->deviceCount);
			printf("default odevice: %d\n", info->defaultOutputDevice);
			printf("default idevice: %d\n", info->defaultInputDevice);
			std::cout << std::endl;
		}
	}
	PaDeviceInfo const *info = Pa_GetDeviceInfo(Pa_GetDefaultOutputDevice());
	if (!info)
	{
		std::cout << "default device not found" << std::endl;
	}
	else
	{
		PaHostApiInfo const *apiinfo = Pa_GetHostApiInfo(info->hostApi);
		printf("Default odevice: %s of API %s\n", info->name, apiinfo->name);
	}
#endif
#endif

	QueueBuffer<float> qb(DEFAULT_SAMPLE_RATE * DEFAULT_CHANNELS * 5);

	std::string dancingQueen{"01 - Dancing Queen.ogg"};
	std::string waterloo{"19 - Waterloo.ogg"};

	OggFileLoader<float, VorbisDecoder> loader;

	loader.open(dancingQueen);

	loader.init();

	for (auto a : loader.getDecoder().getComments())
	{
		std::cout << a << std::endl;
	}
	
	Mixer::SawtoothGenerator<float> sg(440,5, DEFAULT_CHANNELS, DEFAULT_SAMPLE_RATE);
	Mixer::SawtoothGenerator<float> sg2(439,5, DEFAULT_CHANNELS, DEFAULT_SAMPLE_RATE);
	
	DataFlow::Applicator<float,VolumeControl<float>> vc;
	DataFlow::Applicator<float,VolumeControl<float>> vc2;
	vc.getMethod().setVolume(0.3f);
	vc2.getMethod().setVolume(0.3f);

	vc.attach(sg);
	vc2.attach(sg2);

	DataFlow::Consolidator<float, Consolidation::Accumulation> consolidator;
	consolidator.attach(vc);
	consolidator.attach(vc2);

	DataFlow::Applicator<float, Clipping::Hard> applicator;
	applicator.attach(consolidator);

	std::thread fillerThread([&] {
		while (true)
		{
			std::vector<float> tmp(512,0);
			int eaten = 0;
			while(eaten<tmp.size()){
				std::vector<float> doubletmp(tmp.begin()+eaten,tmp.end());
				eaten+= qb.add(doubletmp);
			}
			
			if (qb.isFull())
			{
				std::this_thread::yield();
				std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 1000>>(1));
			}
		}
	});

	// std::thread sizeThread([&] {
	// 	while (true)
	// 	{
	// 		std::cout << "Items in testbuffer: " << qb.size() << std::endl;
	// 		std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 10>>(1));
	// 	}
	// });

	Mixer::SimplePlayer<float, Mixer::PortAudioBackend> player(DEFAULT_CHANNELS, DEFAULT_SAMPLE_RATE);
	player.attach(applicator);
	player.play();

	bool b = true;
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 1>>(4));
		player.pause();
		
		if (b)
		{
			player.attach(applicator);
			sg.setFrequency(880,2.0f,"linear");
		}
		else
		{
			player.attach(applicator);
			sg.setFrequency(440,1.0f,"ease-in-out",5);
		}

		if (b)
			player.play();
		b = !b;
		
	}
//! \todo TODO: extract these tests to actual tests
//TMP: buffer test
//#define BUFFER_TEST
#ifdef BUFFER_TEST
#define TEST_BUFFER_SIZE 90000
	QueueBuffer<float> testbuffer(TEST_BUFFER_SIZE);

	Mixer::SineGenerator<float> testsine(440,DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);

	std::thread fillerThread([&] {
		while (true)
		{
			auto tmp = testsine.get(TEST_BUFFER_SIZE / 10);
			int eaten = 0;
			while(eaten<tmp.size()){
				std::vector<float> doubletmp(tmp.begin()+eaten,tmp.end());
				eaten+= testbuffer.add(doubletmp);
			}
			
			if (testbuffer.isFull())
			{
				std::this_thread::yield();
				std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 1000>>(1));
			}
		}
	});

	std::thread sizeThread([&] {
		while (true)
		{
			std::cout << "Items in testbuffer: " << testbuffer.size() << std::endl;
			std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 10>>(1));
		}
	});

	Mixer::SimplePlayer<float, Mixer::PortAudioBackend> player(DEFAULT_CHANNELS, DEFAULT_SAMPLE_RATE);
	player.attach(testbuffer);
	player.play();


	fillerThread.join();
	sizeThread.join();
#endif

	std::cin.get();
}