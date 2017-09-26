//\todo TODO(later): equalizer

//KONKLUZIÓ : FIX FRAMES-PRE-BUFFER KELL A KATTOGÁS ELLEN (legalábbis pulseaudiónál)

//??? ha nincs buffer adat adjon 0-t? ezzel ki lehet küszöbölni az under és overflowot

//szerintem azért van az underflow startnál, mert az openstreamköltséges?
//con:amig nincs kész addig nem is hívna callbacket

//probléma: mintha kihagyna amikor resume-nál underflow van.

//TODO: separate h+impl

//TODO: channel + samplerate menjen végig (AudioDescriptor?)
//TODO: change template in applicator/consolidator to interface/functor pointer: otherwise you cant change them in runtime

/*!
	\~todo TODO: transitionable<T> - a mutexed(? maybe not cuz time can be indeterminate? just atomic?) generalized that can handle transitioning values
	give it destination value and time and it does its thing
*/

#include <iostream>
#include <thread>
#include <chrono>

#include <Utils.h>

#include <OggFileLoader.h>
#include <SimplePlayer.h>
#include <PortAudioBackend.h>
#include <SineGenerator.h>
#include <Consolidator.h>
#include <ConsolidationMethods.h>
#include <Applicator.h>
#include <Clipping.h>
#include <VolumeControl.h>

#include <QueueBuffer.h>
#include <VorbisDecoder.h>

#define DEFAULT_CHANNELS 2
#define DEFAULT_SAMPLE_RATE 48000

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

	QueueBuffer<float> qb(512 * 16);

	std::string dancingQueen{"01 - Dancing Queen.ogg"};
	std::string waterloo{"19 - Waterloo.ogg"};
	std::string mono{"mono86kbps44100.ogg"};

	OggFileLoader<float, VorbisDecoder> loader;

	loader.open(dancingQueen);

	loader.init();

	for (auto a : loader.getDecoder().getComments())
	{
		std::cout << a << std::endl;
	}

	Mixer::SineGenerator<float> sg(440, DEFAULT_CHANNELS, DEFAULT_SAMPLE_RATE);
	Mixer::SineGenerator<float> sg2(439, DEFAULT_CHANNELS, DEFAULT_SAMPLE_RATE);

	DataFlow::Applicator<float,VolumeControl<float>> vc;
	DataFlow::Applicator<float,VolumeControl<float>> vc2;
	DataFlow::Applicator<float,VolumeControl<float>> vc3;
	vc.getMethod().setVolume(0.3f);
	vc2.getMethod().setVolume(0.3f);
	vc3.getMethod().setVolume(0.3f);
	
	vc.attach(sg);
	vc2.attach(sg2);
	vc3.attach(loader);
	
	DataFlow::Consolidator<float, Consolidation::Accumulation> consolidator;
	consolidator.attach(vc);
	consolidator.attach(vc2);
	consolidator.attach(vc3);
	
	DataFlow::Applicator<float, Clipping::Hard> applicator;
	applicator.attach(consolidator);

	Mixer::SimplePlayer<float, Mixer::PortAudioBackend> player(DEFAULT_CHANNELS, DEFAULT_SAMPLE_RATE);
	player.attach(applicator);
	player.play();

	bool b = true;
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 1>>(3));
		player.pause();

		if (b)
		{
			player.attach(vc);
		}
		else
		{
			player.attach(applicator);
		}

		if (b)
			player.play();
		b = !b;
	}
//! \todo TODO: extract these tests to actual tests
/*
while(true){
	OggFileLoader<float,VorbisDecoder> testloader;
	testloader.open(waterloo);
	testloader.init();
	std::vector<float> v(50,0);
	while(v.size()>0){
		v = testloader.get(2048);
		std::cout<<"Got "<<v.size()<<std::endl;
	}
	std::cin.get();
}
*/
//TMP: buffer test
//#define BUFFER_TEST
#ifdef BUFFER_TEST
#define TEST_BUFFER_SIZE 90000
	QueueBuffer<float> testbuffer(TEST_BUFFER_SIZE);

	player.init(testbuffer, ad);
	player.play();

	std::thread fillerThread([&] {
		while (true)
		{
			for (float i = 0; i < 1; i += 1.0f / TEST_BUFFER_SIZE)
			{
				testbuffer.add(i);
			}
			if (testbuffer.isFull())
			{
				std::this_thread::yield();
				std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 1000>>(1));
			}
		}
	});

	float into;
	float prev = 0;
	std::thread readerThread([&] {
		while (true)
		{
			for (int i = 0; i < TEST_BUFFER_SIZE; i++)
			{
				testbuffer.get(into);
				if (prev + (1.0f / TEST_BUFFER_SIZE) > into && into != 0)
				{
					std::cout << prev << " -> " << into << std::endl;
				}
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
			std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 1000>>(1));
		}
	});

	fillerThread.join();
	readerThread.join();
	sizeThread.join();
#endif

	std::cin.get();
}