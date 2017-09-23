#pragma once

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vector>
#include <string>

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
    std::vector<std::string> getComments() const;
    std::string getVendor() const;
    unsigned int getChannels() const;
    unsigned int getSampleRate() const;

    std::vector<float> decode(ogg_packet & packet);

    void clear();

    VorbisDecoder();
    ~VorbisDecoder();
};