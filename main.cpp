//TODO: frame-sync : bal �s jobb csak egyszerre vehet� ki (std::pair<float,float>)
//-> mono, stereo.... az �sszes csatorn�ra kell megold�st tal�lni
//szétszedni modulokra

#include <iostream>
#include <portaudio.h>
#include <ogg/ogg.h>
#include <fstream>
#include <vector>
#include <vorbis/codec.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include "utils.h"

#include "tmp.h"


std::mutex m;
std::condition_variable cv;

#define SINE_SIZE 200
float sinetable[SINE_SIZE];
int left, right = 0;

float linear_volume = 0.0f;


#include "queue_buffer.cpp"

static int bufferCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	/* Cast data passed through stream to our structure. */
	queue_buffer<float> * buffer = (queue_buffer<float>*)userData;
	float *out = (float*)outputBuffer;
	unsigned int i;
	(void)inputBuffer; /* Prevent unused variable warning. */
	/*
	float into;
	i = 0;
	bool more;
	while (i<framesPerBuffer*2) {
		more = buffer->get(into);
		if (!more) { return paContinue; }
		*out = into;
		out++;
		i++;
	}*/
	float into;
	i = 0;
	bool more;
	while (i<framesPerBuffer) {
		more = buffer->get(into);
		if (!more) { return paContinue; }
		*out = clip::soft_clip(into + sinetable[left] * linear_volume);
		left += 1;
		if (left >= SINE_SIZE) { left = 0; }
		if (*out > 1.0f || *out <-1.0f) { std::cout<< into + sinetable[left] * linear_volume<<"-ra " << *out << std::endl; }
		out++;
		more = buffer->get(into);
		if (!more) { return paContinue; }
		*out = clip::soft_clip(into + sinetable[right] * linear_volume);
		right += 5;
		if (right >= SINE_SIZE) { right = 0; }
		if (*out > 1.0f || *out < -1.0f) { std::cout << *out << std::endl; }
		out++;
		i++;
		//volume of noise
		linear_volume += 0.00001f;
		if (linear_volume > 2.0f) { linear_volume = 0.0f; }
	}
	return paContinue;
}


void play(queue_buffer<float> *qb,const audio_descriptor * ad,bool * condition) {

#ifdef DEBUG_SINE_PLAY
	std::ofstream out_sine;
	out_sine.open("sine.txt");

	for (int j = 0; j < length; j=j+2) {
		int spaces = -50;

		for (int s = -50; s < data[j] * 50; s++) {
			out_sine << " ";
			spaces++;
			if (s == 0) { out_sine << "|"; }
		}
		out_sine << "#" << data[j];
		while (spaces < 100) {
			out_sine << " ";
			if (spaces == 0) { out_sine << "|"; }
			spaces++;
		}

		spaces = -50;

		for (int s = -50; s < data[j+1] * 50; s++) {
			out_sine << " ";
			spaces++;
			if (s == 0) { out_sine << "|"; }
		}
		out_sine << "#" << data[j+1];
		while (spaces < 100) {
			out_sine << " ";
			if (spaces == 0) { out_sine << "|"; }
			spaces++;
		}
		out_sine << std::endl;
	}
	out_sine.close();
#endif

#define PA_SAMPLE_TYPE  paFloat32;

	PaStreamParameters outputParameters;
	PaStream *stream;
	PaError err;

	err = Pa_Initialize();
	if (err != paNoError) goto error;

	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	if (outputParameters.device == paNoDevice) {
		fprintf(stderr, "Error: No default output device.\n");
		goto error;
	}
	outputParameters.channelCount = ad->channels;
	outputParameters.sampleFormat = PA_SAMPLE_TYPE;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	printf("Begin playback.\n"); fflush(stdout);
	err = Pa_OpenStream(
		&stream,
		NULL, /* no input */
		&outputParameters,
		ad->sample_rate,
		0,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		bufferCallback, /* no callback, use blocking API */
		qb); /* no callback, so no callback userData */
	if (err != paNoError) goto error;

 

	if (stream)
	{
		err = Pa_StartStream(stream);
		if (err != paNoError) goto error;
		printf("Waiting for playback to finish.\n"); fflush(stdout);

		std::unique_lock<std::mutex> lk(m);
		while (!*condition) {
			cv.wait(lk);
		}
		while (!qb->isEmpty()) {}
		err = Pa_StopStream(stream);
		err = Pa_CloseStream(stream);

		if (err != paNoError) goto error;
		printf("Done.\n"); fflush(stdout);
	}

	Pa_Terminate();

	return;

error:
	Pa_Terminate();
	fprintf(stderr, "An error occured while using the portaudio stream\n");
	fprintf(stderr, "Error number: %d\n", err);
	fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));

}


#include <string>

int main() {

	std::cout<<Pa_GetVersionText()<<std::endl;
PaDeviceInfo const* info = Pa_GetDeviceInfo(0);
    if (!info) {std::cout<<"device not found"<<std::endl;}
	else{
    printf("%s\n", info->name);
	}
	
	for (int i = 0; i < SINE_SIZE; i++) {
		sinetable[i] = (float)sin(((double)i / (double)SINE_SIZE) * 3.14f * 2.);
	}

	queue_buffer<float> qb(4096);

	bool done_loading = false;

	audio_descriptor ad{};
	std::thread t1(load, &qb, &ad, &done_loading, &cv,&m);
	//kellene ready to play CV - mi van ha nem olvasta még be a sample ratet?
	std::thread t2(play, &qb, &ad, &done_loading);
	t2.join();
	t1.join();
	std::cout << "load finished" << std::endl;
	//std::cout << played << "," << read << std::endl;

	std::cout << "threads finished" << std::endl;
	std::cin.get();
}