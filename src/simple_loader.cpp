#include <SimpleLoader.h>
#include "utils.h"

#define BUFFERSIZE 4096

template <typename T>
void simple_loader<T>::load(iBuffer<T> & _buffer ,  std::promise<audio_descriptor> _adp){
    //TODO:exceptions

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
		infile.read(buffer, BUFFERSIZE);
		bytes = infile.gcount();
		ogg_sync_wrote(&oy, bytes);

		/* Get the first page. */
		if (ogg_sync_pageout(&oy, &og) != 1) {
			/* have we simply run out of data?  If so, we're done. */
			if (bytes<BUFFERSIZE)break;

			/* error case.  Must not be Vorbis data */
			fprintf(stderr, "Input does not appear to be an Ogg bitstream.\n");
			//ERROR
            return;
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
			//ERROR
            return;
		}

		if (ogg_stream_packetout(&os, &op) != 1) {
			/* no page? must not be vorbis */
			fprintf(stderr, "Error reading initial header packet.\n");
			//ERROR
            return;
		}

		if (vorbis_synthesis_headerin(&vi, &vc, &op)<0) {
			/* error case; not a vorbis header */
			fprintf(stderr, "This Ogg bitstream does not contain Vorbis "
				"audio data.\n");
			//ERROR
            return;
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
                            //ERROR
                            return;
						}
						result = vorbis_synthesis_headerin(&vi, &vc, &op);
						if (result<0) {
							fprintf(stderr, "Corrupt secondary header.  Exiting.\n");
                            //ERROR
                            return;
						}
						i++;
					}
				}
			}
			/* no harm in not checking before adding more */
			buffer = ogg_sync_buffer(&oy, BUFFERSIZE);
			infile.read(buffer, BUFFERSIZE);
			bytes = infile.gcount();
			if (bytes == 0 && i<2) {
				fprintf(stderr, "End of file before finding all Vorbis headers!\n");
                //ERROR
                return;
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

			audio_descriptor ad (vi.channels,vi.rate);

			_adp.set_value(ad);

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
											int res = _buffer.add(pcm[j][i]);
											//debug::read++;
											while (res<0) {
												std::this_thread::yield();
												std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 1000>>(1));
												res = _buffer.add(pcm[j][i]);
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
					infile.read(buffer, BUFFERSIZE);
					bytes = infile.gcount();
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

}

template <typename T>
bool simple_loader<T>::open(const std::string & _filename){
    this->infile.open(_filename.c_str(),std::ios::binary);
    if(!infile.is_open()){
        //error
        std::cerr<<"Error opening file: "<<_filename.c_str() << std::endl;
        return false;
    }
    return true;
}

template<typename T>
bool simple_loader<T>::close(){
	if(this->infile.close())
		return true;
	return false;
}

template <typename T>
void simple_loader<T>::pause_load(){
    
}

template <typename T>
simple_loader<T>::~simple_loader(){
    if(this->infile.is_open()){
		this->infile.close();
	}
}