//!\todo TODO:microphone
//!\todo TODO:modulating filter
//!\todo TODO: file endpoint

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#include <Utils.h>

#include <OggFileLoader.h>
#include <SimplePlayer.h>
#include <PortAudioBackend.h>
#include <SineGenerator.h>
#include <SawtoothGenerator.h>
#include <TriangleGenerator.h>
#include <CheapSquareGenerator.h>
#include <CheapSawtoothGenerator.h>
#include <SquareGenerator.h>
#include <Consolidator.h>
#include <ConsolidationMethods.h>
#include <Applicator.h>
#include <Clipping.h>
#include <VolumeControl.h>
#include <SimpleNote.h>

#include <VorbisEncoder.h>

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
	
	Mixer::SquareGenerator<float> sg(227.2,150,DEFAULT_CHANNELS, DEFAULT_SAMPLE_RATE);
	Mixer::SineGenerator<float> sg2(80, DEFAULT_CHANNELS, DEFAULT_SAMPLE_RATE);
	
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

	Mixer::SimpleNote<float,Transition::Linear,Transition::EaseOut::Cubic> note(0.2f,DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);
	note.attach(sg2);
	note.setAttack(0.1f);
	note.setDecay(0.12f);
	note.bakeNote();

	
	DataFlow::Applicator<float,VolumeControl<float>> lower;
	lower.getMethod().setVolume(0.5f);
	lower.attach(note);
	
	
{
	VorbisEncoder encoder(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE,1);
	encoder.open("test.ogg");
	encoder.addComment(std::make_pair ("test","comment"));
	encoder.initEncoding();
	int c = 0;
	while(c<DEFAULT_CHANNELS*DEFAULT_SAMPLE_RATE*10){
		encoder.add(std::vector<float>(DEFAULT_CHANNELS*DEFAULT_SAMPLE_RATE*0.5f));
		c+=DEFAULT_CHANNELS*DEFAULT_SAMPLE_RATE*0.5f;
		auto vec = lower.get(note.getLength());
		c+=vec.size();
		encoder.add(vec);
	}
	encoder.close();
	std::cout<<"Done generating."<<std::endl;
}
	OggFileLoader<float, VorbisDecoder> loader;
	
	loader.open("test.ogg");
	
	loader.init();
	
	for (auto a : loader.getDecoder().getComments())
	{
		std::cout << a << std::endl;
	}

	Mixer::SimplePlayer<float, Mixer::PortAudioBackend> player(DEFAULT_CHANNELS, DEFAULT_SAMPLE_RATE);
	player.attach(loader);
	player.play();


	// bool b = true;
	// int counter = 1;
	// while (true)
	// {
	// 	std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 1>>(3));
	// 	player.pause();
		
	// 	if (b)
	// 	{
	// 		player.attach(applicator);
	// 		sg.setFrequency(220,1.0f);
	// 	}
	// 	else
	// 	{
	// 		player.attach(applicator);
	// 		sg.setFrequency(880,1.0f,"ease-out",4);
	// 	}

	// 	player.play();
	// 	b = !b;
	// 	/*switch (counter){
	// 		case 1 :
	// 		sg.setFrequency(880);
	// 		break;
	// 		case 2 :
	// 		sg.setFrequency(990);
	// 		break;
	// 		case 3 :
	// 		sg.setFrequency(1760);
	// 		break;
	// 		case 4 :
	// 		sg.setFrequency(1870);
	// 		break;		
	// 	}
	// 	counter = (counter + 1) % 5;
	// 	*/
	// }

	std::cin.get();
}