#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <vector>

#include <ogg/ogg.h>
#include <vorbis/codec.h>

#include <iLoader.h>
#include <Utils.h>
#include <VorbisDecoder.h>

template <typename T,typename K>
class SimpleLoader : public iLoader<T>{
private:
    std::ifstream infile;
    bool eos = true;
    char * buffer;

    ogg_sync_state   oy; /* sync and verify incoming physical bitstream */    
    ogg_stream_state os; /* take physical pages, weld into a logical stream of packets */

    ogg_page         currentPage; /* one Ogg bitstream page. Vorbis packets are inside */
    ogg_packet       currentPacket; /* one raw packet of data for decode */

    K decoder;

    bool streamInitialized = false;
    bool initialized = false;

    int initStream(ogg_page & page);
    int read();
    int readNextPage();
    int nextPage();
    int nextPacket();

public:
    void load(DataFlow::iBuffer<T> & _buffer , std::promise<audio_descriptor> _adp);
    bool open(const std::string & _filename);
    bool close();
    void clear();
    void pauseLoad();
    std::vector<T> get(const unsigned int & amount);    

    SimpleLoader<T,K>();
	~SimpleLoader<T,K>();	
};