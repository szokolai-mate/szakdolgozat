//TODO: frame-sync : bal �s jobb csak egyszerre vehet� ki (std::pair<float,float>)
//-> mono, stereo.... az �sszes csatorn�ra kell megold�st tal�lni
//szétszedni modulokra

//úgy tűnik nekem kell egy real-time mixert csinálnom. fuck.

//TODO(later): equalizer

//TODO: blokkolóval tesztelni a sinuszt

//TODO: statikus buffer, hogy ellenőrizhessem a szekvencialitást és hogy a pause/play miért dobál el frameket (gyorsabbnak tűnik a lejátszás)

//pretest: blokkoló example nem kattog, nem blokkoló ugyanugy kattog.
//BLOKKOLÓVAL KELL AZ EGÉSZET CSINÁLNOM GÓFUCK
//pretest2: a nem blokkoló record-playback nem mutat poppolást, KELL MÉG TESZTELNI
//pretest3: real time visszaáttszás is kattog.
//KONKLUZIÓ: pulseaudio bekaphatja

/*ha direkt kapcsolódok a pcm playback device-ra, a kattogásnak végre vvége.
tehát pulseaudio = el kell foglalni a hardvert = semmi más nem adhat ki hangot közben
=> pulseaudio detected = blokkoló irás nem callback*/

//KONKLUZIÓ : FIX FRAMES-PRE-BUFFER KELL A KATTOGÁS ELLEN (legalábbis pulseaudiónál)

//??? ha nincs buffer adat adjon 0-t? ezzel ki lehet küszöbölni az under és overflowot

//szerintem azért van az underflow startnál, mert az openstreamköltséges?
//con:amig nincs kész addig nem is hívna callbacket

//probléma: mintha kihagyna amikor resume-nál underflow van.

//TODO:probléma:pause és play kiugraszt cuccot valamiért.
//VOLUME: valahogy a callbackbe rakni a volume-babrálást? (currentVolume, targetVolume,volumeChange)

//tézis: a portaudio stopstream-je alapból kattan valamiért.
//TODO: read up on move semantics for efficient buffer mixing/input/output

//CURRENT TODO: separate h+impl -> now linker errors

//BUGHUNT: in player after getting out of the buffer: random discrepencies: -0.1 -> -0.6 -> -0.1
// ->must do a buffer test
//		->buffer seems ok, even when read and write are in different threads
//talán a mutex lockolgatás miatt van?

//TODO: eldönteni ki a felelős azért hogy ne framesPerBuffer hanem ftamesPerBuffer*channels sample follyon

#include <iostream>
#include <thread>
#include <future>

#include <portaudio.h>

#include <Utils.h>

#include <OggFileLoader.h>
#include <PortAudioBackend.h>
#include <SineGenerator.h>
//#include "simple_manager.cpp"

#include <QueueBuffer.h>
#include <VorbisDecoder.h>

#include <set>
#include <chrono>

int main()
{
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

QueueBuffer<float> qb(512 * 16);


	std::string dancingQueen{"01 - Dancing Queen.ogg"};
	std::string waterloo{"19 - Waterloo.ogg"};
	std::string mono{"mono86kbps44100.ogg"};

	OggFileLoader<float,VorbisDecoder> loader;
	OggFileLoader<float,VorbisDecoder> loader2;

	loader.open(dancingQueen);
	loader2.open(waterloo);
	
	loader.init();
	loader2.init();

	Mixer::SineGenerator<float> sg(440);

	PortAudioBackend<float> player;
	player.init(loader2, 2, 44100);
	player.start();

	bool b = true;
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 1>>(3));
		player.stop();
		if (b)

		//MEMORY LEAK HERE
		{
			player.init(loader, 2, 44100);
		}
		else
		{
			player.init(loader2, 2, 44100);
		}
		//
		player.start();
		b = !b;
	}

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