#include <OggFileLoader.h>

#define BUFFERSIZE 4096

template <typename T, typename K>
const K & OggFileLoader<T, K>::getDecoder(){
	return decoder;
}

template <>
int OggFileLoader<float, VorbisDecoder>::init()
{
	for (int i = 0; i < 3; i++)
	{
		nextPacket();
		decoder.addToHeader(currentPacket);
	}
	return decoder.initDecoding();
}

template <typename T, typename K>
int OggFileLoader<T, K>::initStream(ogg_page &page)
{
	int err = ogg_stream_init(&os, ogg_page_serialno(&page));
	if (err != 0)
	{
		//ERROR
		return err;
	}
	streamInitialized = true;
}

template <typename T, typename K>
int OggFileLoader<T, K>::read()
{
	buffer = ogg_sync_buffer(&oy, BUFFERSIZE);
	infile.read(buffer, BUFFERSIZE); //read into buffer
	int bytes = infile.gcount();	 //calculate how much we wrote into buffer
	ogg_sync_wrote(&oy, bytes);		 //tell ogg how much we wrote to buffer
	return bytes;
}

template <typename T, typename K>
int OggFileLoader<T, K>::readNextPage()
{
	int err = ogg_sync_pageout(&oy, &currentPage);
	while (err != 1 && !eos)
	{
		read();
		err = ogg_sync_pageout(&oy, &currentPage);
		//return err;
	}
	return err;
}

template <typename T, typename K>
int OggFileLoader<T, K>::nextPage()
{
	int err = readNextPage();
	if (!streamInitialized)
	{
		initStream(currentPage);
	}
	if (err != 1)
	{
		return err;
	}
	return ogg_stream_pagein(&os, &currentPage);
}

template <typename T, typename K>
int OggFileLoader<T, K>::nextPacket()
{
	if (!streamInitialized)
		nextPage();
	int err = ogg_stream_packetout(&os, &currentPacket);
	while (err != 1 && !eos)
	{
		nextPage();
		err = ogg_stream_packetout(&os, &currentPacket);
	}
	return err;
}

template <typename T, typename K>
std::vector<T> OggFileLoader<T, K>::get(const unsigned int &amount)
{
	std::vector<T> res;
	res.reserve(amount);
	int samples = 0;
	while (samples < amount)
	{
		if(overflow.size()>0){
			int amountFits = std::min(amount - samples, (unsigned int)overflow.size());	
			res.insert(res.end(), overflow.begin(), overflow.begin()+amountFits);			
			samples+=amountFits;
			overflow = std::vector<T>(overflow.begin()+amountFits,overflow.end());			
			continue;			
		}
		int err = nextPacket();
		if(err!=1){
			eos=true;
			return res;
		}
		std::vector<T> decoded = decoder.decode(currentPacket);
		int amountFits = std::min(amount - samples, (unsigned int)decoded.size());
		res.insert(res.end(), decoded.begin(), decoded.begin()+amountFits);
		samples+=amountFits;
		overflow = std::vector<T>(decoded.begin()+amountFits,decoded.end());
	}
	return res;
}

template <typename T, typename K>
bool OggFileLoader<T, K>::open(const std::string &_filename)
{
	this->infile.open(_filename.c_str(), std::ios::binary);
	if (!infile.is_open())
	{
		//error
		return false;
	}
	eos = false;
	return true;
}

template <typename T, typename K>
bool OggFileLoader<T, K>::close()
{
	this->infile.close();
	eos = true;
	return !this->infile.fail();
}

template <typename T, typename K>
void OggFileLoader<T, K>::clear()
{
	ogg_stream_clear(&os);
	ogg_sync_clear(&oy);
}

template <typename T, typename K>
OggFileLoader<T, K>::OggFileLoader()
{
	ogg_sync_init(&oy);
}

template <typename T, typename K>
OggFileLoader<T, K>::~OggFileLoader()
{
	if (this->infile.is_open())
	{
		this->infile.close();
	}
	clear();
}

template class OggFileLoader<float, VorbisDecoder>;