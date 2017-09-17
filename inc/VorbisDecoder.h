#pragma once

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vector>

//tmp
#include <iostream>

class VorbisDecoder
{
  private:
    vorbis_info vi;      /* struct that stores all the static vorbis bitstream settings */
    vorbis_comment vc;   /* struct that stores all the bitstream user comments */
    vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
    vorbis_block vb;     /* local working space for packet->PCM decode */
  public:
    int addToHeader(ogg_packet & packet);
    int initDecoding();
    std::vector<std::string> getComments();
    std::string getVendor();
    unsigned int getChannels();
    unsigned int getSampleRate();

    std::vector<float> decode(ogg_packet & packet);

    void clear();
    
    void printHeader(){
      for(auto s : getComments()){
        std::cout<<s<<std::endl;
      }
      std::cout<<"Bitsream is "<<getSampleRate()<<" on "<<getChannels()<<" channels."<<std::endl;
      std::cout<<"Encoded by "<<getVendor()<<std::endl;
    }

    VorbisDecoder();
    ~VorbisDecoder();
};