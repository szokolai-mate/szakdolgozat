#pragma once

#include <fstream>
#include <string>
#include <vector>

#include <ogg/ogg.h>

#include <iSource.h>
#include <Utils.h>
#include <VorbisDecoder.h>

template <typename T,typename K>
class OggFileLoader : public DataFlow::iSource<T>{
private:
    std::ifstream infile;
    bool eos = true;
    char * buffer;

    ogg_sync_state   oy; /* sync and verify incoming physical bitstream */    
    ogg_stream_state os; /* take physical pages, weld into a logical stream of packets */

    ogg_page         currentPage; /* one Ogg bitstream page. Vorbis packets are inside */
    ogg_packet       currentPacket; /* one raw packet of data for decode */

    K decoder;

    std::vector<T> overflow;

    bool streamInitialized = false;

    int initStream(ogg_page & page);
    int read();
    int readNextPage();
    int nextPage();
    int nextPacket();

public:
    bool open(const std::string & _filename);
    int init();    
    bool close();
    void clear();
    std::vector<T> get(const unsigned int & amount);    
    

    OggFileLoader<T,K>();
	~OggFileLoader<T,K>();	
};