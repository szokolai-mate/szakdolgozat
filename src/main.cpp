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

#include <iostream>
#include <thread>
#include <future>

#include <portaudio.h>

//#include "utils.h"
#include <iBuffer.h>

//#include <SimpleLoader.h>
//#include "simple_manager.cpp"

//#include "queue_buffer.cpp"

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

	//queue_buffer<float> qb(512*16);
	//simple_loader<float> loader;

	std::string elephant{"Ain't_No_Rest_For_The_Wicked.ogg"};
	std::string abba{"01 - Dancing Queen.ogg"};
	std::string mono{"mono86kbps44100.ogg"};
	//loader.open(abba);

	/*std::promise<audio_descriptor> adp;
    std::future<audio_descriptor> adf = adp.get_future();
    std::thread t1([&] { loader.load(qb, std::move(adp)); });
	
	simple_manager<simple_player<float>> mt;

    adf.wait();

	mt.init(qb, adf.get());
	mt.play();

    bool l=true;
    std::cout<<"Press any key to play/pause"<<std::endl;
    while(std::cin.get()){
        if(l)mt.pause();
        else mt.play();
        l=!l;
    }

    t1.join();
    std::cout << "threads finished" << std::endl;*/
	std::cin.get();
}