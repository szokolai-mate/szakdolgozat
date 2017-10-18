//!\todo TODO:modulating filter

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
#include <ComplexWaveformGenerator.h>
#include <SquareGenerator.h>
#include <Consolidator.h>
#include <ConsolidationMethods.h>
#include <Applicator.h>
#include <Clipping.h>
#include <VolumeControl.h>
#include <SimpleNote.h>
#include <Multiplexer.h>

#include <VorbisEncoder.h>

#include <RepeatingBuffer.h>
#include <QueueBuffer.h>
#include <VorbisDecoder.h>
#include <Recorder.h>

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
	
	Mixer::SineGenerator<float> sg(220,1, DEFAULT_SAMPLE_RATE);
	Mixer::SineGenerator<float> sg2(880, 1, DEFAULT_SAMPLE_RATE);
	DataFlow::QueueBuffer<float> buffer;

{
	OggFileLoader<float, VorbisDecoder> queen;
	
	queen.open("01 - Dancing Queen.ogg");
	
	queen.init();
	
	for (auto a : queen.getDecoder().getComments())
	{
		std::cout << a << std::endl;
	}

	OggFileLoader<float, VorbisDecoder> water;
	
	water.open("19 - Waterloo.ogg");
	
	water.init();
	
	for (auto a : water.getDecoder().getComments())
	{
		std::cout << a << std::endl;
	}

	float firstHarmonicFrequency = 110;
	std::vector<std::pair<float,float>> components;
	components.push_back(std::make_pair(firstHarmonicFrequency,1));
	components.push_back(std::make_pair(firstHarmonicFrequency*2,0.5f));
	components.push_back(std::make_pair(firstHarmonicFrequency*3,0.33f));
	components.push_back(std::make_pair(firstHarmonicFrequency*4,0.25f));
	components.push_back(std::make_pair(firstHarmonicFrequency*2.44f,0.2f));
	components.push_back(std::make_pair(firstHarmonicFrequency*2.76f,0.4f));
	
	Mixer::ComplexWaveformGenerator<float> complexGenerator(components,DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE);

	DataFlow::Applicator<float,VolumeControl<float>> vc;
	DataFlow::Applicator<float,VolumeControl<float>> vc2;
	vc.getMethod().setVolume(0.3f);
	vc2.getMethod().setVolume(0.3f);
	
	vc.attach(queen);
	vc2.attach(water);
	
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
	lower.getMethod().setVolume(0.3f);
	lower.attach(note);
	consolidator.attach(lower);	

	int c = 0;
	int totalLength = DEFAULT_CHANNELS*DEFAULT_SAMPLE_RATE*10;
	bool going = false;
	while(buffer.size()<totalLength){
		//std::vector<float> silence(DEFAULT_CHANNELS*DEFAULT_SAMPLE_RATE*0.5f,0);
		//buffer.put(silence);
		/*if(c>totalLength/4 && !going){
			complexGenerator.setComponent(firstHarmonicFrequency,0,1,"ease-in-out",5);
			complexGenerator.setComponent(firstHarmonicFrequency*2,0,1);
			complexGenerator.setComponent(firstHarmonicFrequency*3,0,1);
			complexGenerator.setComponent(firstHarmonicFrequency*4,0,1);
			going = true;
		}*/
		buffer.put(applicator.get(512));
	}
}
{
	VorbisEncoder encoder(DEFAULT_CHANNELS,DEFAULT_SAMPLE_RATE,1);
	encoder.open("test.ogg");
	encoder.addComment(std::make_pair ("test","comment"));
	encoder.initEncoding();
	while(buffer.size()>512){
		encoder.add(buffer.get(512));
	}
	encoder.add(buffer.get(buffer.size()));
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
	player.attach(mx);
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