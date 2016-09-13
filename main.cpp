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
#include <math.h>

std::mutex m;
std::condition_variable cv;

int played = 0, read = 0;

#define SINE_SIZE 200
float sinetable[SINE_SIZE];
int left, right = 0;

float linear_volume = 0.0f;

inline float soft_clip(float x) {
	//az ellenőrzést lehet hogy kijjebb kellene tenni
	if (x > 0.8f || x < -0.8f) {
		return ( (pow(exp(1),(exp(1)*x)) - 1) / (pow(exp(1), (exp(1)*x)) + 1) );
	}
	else return x;
}

struct audio_descriptor {
	unsigned short channels;
	unsigned short sample_rate;

	audio_descriptor() {}
	audio_descriptor(unsigned short _channels, unsigned short _sample_rate) {
		channels = _channels;
		sample_rate = _sample_rate;
	}
};


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
		*out = soft_clip(into + sinetable[left] * linear_volume);
		left += 1;
		if (left >= SINE_SIZE) { left = 0; }
		if (*out > 1.0f || *out <-1.0f) { std::cout<< into + sinetable[left] * linear_volume<<"-ra " << *out << std::endl; }
		out++;
		more = buffer->get(into);
		if (!more) { return paContinue; }
		*out = soft_clip(into + sinetable[right] * linear_volume);
		right += 5;
		if (right >= SINE_SIZE) { right = 0; }
		if (*out > 1.0f || *out < -1.0f) { std::cout << *out << std::endl; }
		out++;
		i++;
		//volume of noise
		//linear_volume += 0.00001f;
		if (linear_volume > 2.0f) { linear_volume = 0.0f; }
	}
	return paContinue;
}


void play(queue_buffer<float> *qb,audio_descriptor * ad,bool * condition) {

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

int load(queue_buffer<float> * qb,bool * conditon) {

#define BUFFERSIZE 4096

	std::ifstream in;

	//in.open("Sine.ogg", std::ios::binary);
	//in.open("01 - Dancing Queen.ogg", std::ios::binary);
	//in.open("Lumme-Badloop.ogg", std::ios::binary);
	//in.open("mono.ogg", std::ios::binary);
	in.open("Ain't_No_Rest_For_The_Wicked.ogg", std::ios::binary);
	if(!in.is_open()){std::cout<<"error opening audio file"<<std::endl;}
	
	ogg_sync_state   oy; /* sync and verify incoming physical bitstream */
	ogg_stream_state os; /* take physical pages, weld into a logical
						 stream of packets */
	ogg_page         og; /* one Ogg bitstream page. Vorbis packets are inside */
	ogg_packet       op; /* one raw packet of data for decode */

	vorbis_info      vi; /* struct that stores all the static vorbis bitstream
						 settings */
	vorbis_comment   vc; /* struct that stores all the bitstream user comments */
	vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
	vorbis_block     vb; /* local working space for packet->PCM decode */

	char *buffer;
	int  bytes;

	int channels;
	int sample_rate;

	ogg_sync_init(&oy);

	while (1) { /* we repeat if the bitstream is chained */
		int eos = 0;
		int i;

		/* grab some data at the head of the stream. We want the first page
		(which is guaranteed to be small and only contain the Vorbis
		stream initial header) We need the first page to get the stream
		serialno. */

		/* submit a 4k block to libvorbis' Ogg layer */
		buffer = ogg_sync_buffer(&oy, BUFFERSIZE);
		in.read(buffer, BUFFERSIZE);
		bytes = in.gcount();
		ogg_sync_wrote(&oy, bytes);

		/* Get the first page. */
		if (ogg_sync_pageout(&oy, &og) != 1) {
			/* have we simply run out of data?  If so, we're done. */
			if (bytes<BUFFERSIZE)break;

			/* error case.  Must not be Vorbis data */
			fprintf(stderr, "Input does not appear to be an Ogg bitstream.\n");
			exit(1);
		}

		/* Get the serial number and set up the rest of decode. */
		/* serialno first; use it to set up a logical stream */
		ogg_stream_init(&os, ogg_page_serialno(&og));

		/* extract the initial header from the first page and verify that the
		Ogg bitstream is in fact Vorbis data */

		/* I handle the initial header first instead of just having the code
		read all three Vorbis headers at once because reading the initial
		header is an easy way to identify a Vorbis bitstream and it's
		useful to see that functionality seperated out. */
		vorbis_info_init(&vi);
		vorbis_comment_init(&vc);
		if (ogg_stream_pagein(&os, &og)<0) {
			/* error; stream version mismatch perhaps */
			fprintf(stderr, "Error reading first page of Ogg bitstream data.\n");
			exit(1);
		}

		if (ogg_stream_packetout(&os, &op) != 1) {
			/* no page? must not be vorbis */
			fprintf(stderr, "Error reading initial header packet.\n");
			exit(1);
		}

		if (vorbis_synthesis_headerin(&vi, &vc, &op)<0) {
			/* error case; not a vorbis header */
			fprintf(stderr, "This Ogg bitstream does not contain Vorbis "
				"audio data.\n");
			exit(1);
		}



		/* At this point, we're sure we're Vorbis. We've set up the logical
		(Ogg) bitstream decoder. Get the comment and codebook headers and
		set up the Vorbis decoder */

		/* The next two packets in order are the comment and codebook headers.
		They're likely large and may span multiple pages. Thus we read
		and submit data until we get our two packets, watching that no
		pages are missing. If a page is missing, error out; losing a
		header page is the only place where missing data is fatal. */

		i = 0;
		while (i<2) {
			while (i<2) {
				int result = ogg_sync_pageout(&oy, &og);
				if (result == 0)break; /* Need more data */
									   /* Don't complain about missing or corrupt data yet. We'll
									   catch it at the packet output phase */
				if (result == 1) {
					ogg_stream_pagein(&os, &og); /* we can ignore any errors here
												 as they'll also become apparent
												 at packetout */
					while (i<2) {
						result = ogg_stream_packetout(&os, &op);
						if (result == 0)break;
						if (result<0) {
							/* Uh oh; data at some point was corrupted or missing!
							We can't tolerate that in a header.  Die. */
							fprintf(stderr, "Corrupt secondary header.  Exiting.\n");
							exit(1);
						}
						result = vorbis_synthesis_headerin(&vi, &vc, &op);
						if (result<0) {
							fprintf(stderr, "Corrupt secondary header.  Exiting.\n");
							exit(1);
						}
						i++;
					}
				}
			}
			/* no harm in not checking before adding more */
			buffer = ogg_sync_buffer(&oy, BUFFERSIZE);
			in.read(buffer, BUFFERSIZE);
			bytes = in.gcount();
			if (bytes == 0 && i<2) {
				fprintf(stderr, "End of file before finding all Vorbis headers!\n");
				exit(1);
			}
			ogg_sync_wrote(&oy, bytes);
		}

		/* Throw the comments plus a few lines about the bitstream we're
		decoding */
		{
			char **ptr = vc.user_comments;
			while (*ptr) {
				fprintf(stderr, "%s\n", *ptr);
				++ptr;
			}

			channels = vi.channels;
			sample_rate = vi.rate;
			fprintf(stderr, "\nBitstream is %d channel, %ldHz\n", vi.channels, vi.rate);
			fprintf(stderr, "Encoded by: %s\n\n", vc.vendor);
		}

		/* OK, got and parsed all three headers. Initialize the Vorbis
		packet->PCM decoder. */
		if (vorbis_synthesis_init(&vd, &vi) == 0) { /* central decode state */
			vorbis_block_init(&vd, &vb);          /* local state for most of the decode
												  so multiple block decodes can
												  proceed in parallel. We could init
												  multiple vorbis_block structures
												  for vd here */

												  /* The rest is just a straight decode loop until end of stream */
			while (!eos) {
				while (!eos) {
					int result = ogg_sync_pageout(&oy, &og);
					if (result == 0)break; /* need more data */
					if (result<0) { /* missing or corrupt data at this page position */
						fprintf(stderr, "Corrupt or missing data in bitstream; "
							"continuing...\n");
					}
					else {
						ogg_stream_pagein(&os, &og); /* can safely ignore errors at
													 this point */
						while (1) {
							result = ogg_stream_packetout(&os, &op);

							if (result == 0)break; /* need more data */
							if (result<0) { /* missing or corrupt data at this page position */
											/* no reason to complain; already complained above */
							}
							else {
								/* we have a packet.  Decode it */
								float **pcm;
								int samples;

								if (vorbis_synthesis(&vb, &op) == 0) /* test for success! */
									vorbis_synthesis_blockin(&vd, &vb);
								/*

								**pcm is a multichannel float vector.  In stereo, for
								example, pcm[0] is left, and pcm[1] is right.  samples is
								the size of each channel.  Convert the float values
								(-1.<=range<=1.) to whatever PCM format and write it out */
								
								while ((samples = vorbis_synthesis_pcmout(&vd, &pcm))>0) {
									for (int i = 0; i < samples; i++) {
										for (int j = 0; j < vi.channels; j++) {
											int res = qb->add(pcm[j][i]);
											read++;
											while (res<0) {
												std::this_thread::yield();
												std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 1000>>(3));
												res = qb->add(pcm[j][i]);
											}
										}
									}
								/* tell libvorbis how many samples we actually consumed */
									vorbis_synthesis_read(&vd, samples);
								
								}

							}
						}
						if (ogg_page_eos(&og))eos = 1;
					}
				}

				if (!eos) {
					buffer = ogg_sync_buffer(&oy, BUFFERSIZE);
					in.read(buffer, BUFFERSIZE);
					bytes = in.gcount();
					ogg_sync_wrote(&oy, bytes);
					if (bytes == 0)eos = 1;
				}
			}

			/* ogg_page and ogg_packet structs always point to storage in
			libvorbis.  They're never freed or manipulated directly */

			vorbis_block_clear(&vb);
			vorbis_dsp_clear(&vd);
		}
		else {
			fprintf(stderr, "Error: Corrupt header during playback initialization.\n");
		}

		/* clean up this logical bitstream; before exit we see if we're
		followed by another [chained] */

		ogg_stream_clear(&os);
		vorbis_comment_clear(&vc);
		vorbis_info_clear(&vi);  /* must be called last */
	}

	ogg_sync_clear(&oy);
	std::cout << std::endl << "Done loading!" << std::endl;

	//play(qb, channels, sample_rate);
	//*conditon = true;
	std::unique_lock<std::mutex> lk(m);
	*conditon = true;
	cv.notify_all();
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

	bool end = false;

	std::thread t1(load, &qb, &end);
	std::thread t2(play, &qb, new audio_descriptor(2, 48000), &end);
	t2.join();
	t1.join();
	std::cout << "load finished" << std::endl;
	std::cout << played << "," << read << std::endl;

	std::cout << "threads finished" << std::endl;

	std::cin.get();
	//t1.join();
	//t2.join();
}