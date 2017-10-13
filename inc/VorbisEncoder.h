#pragma once

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vector>
#include <string>
#include <fstream>

//! \todo: TODO: documentation
//! \todo: TODO: exceptions and errors

/*! \~english Class used for encoding data into Vorbis packets.
    \~hungarian Vorbis kódolású adatot dekódoló osztály.
*/
class VorbisEncoder{
private:
    std::ofstream outfile;

    unsigned int channels, sampleRate;

    ogg_stream_state os; /* take physical pages, weld into a logical
                            stream of packets */
    ogg_page         og; /* one Ogg bitstream page.  Vorbis packets are inside */
    ogg_packet       op; /* one raw packet of data for decode */

    vorbis_info vi;      /* struct that stores all the static vorbis bitstream settings */
    vorbis_comment vc;   /* struct that stores all the bitstream user comments */
    vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
    vorbis_block vb;     /* local working space for packet->PCM decode */

    int eos = 0;
public:
     /*! \~english Add the comment to the header.

     Comments are a pair of strings, a key and a value.
        \~hungarian A komment hozzáadása a fejléchez.

        A kommentek két string-ből állnak, a kulcs és az érték.
        \~english \param packet the comment to add
                      \~hungarian \param packet a hozzáadandó komment
        \~english \return error code from libvorbis
                \~hungarian \return hibakód libvorbis-tól
    */
    void addComment(std::pair<std::string,std::string> comment);
    /*! \~english Initialize the encoding state. The header needs to be complete.
        \~hungarian A kódolási környezet inicializálása. A fejlécnek teljesnek kell lennie.
        \~english \return error code from libvorbis
                \~hungarian \return hibakód libvorbis-tól
    */
    int initEncoding();

    //int encode();

    int add(const std::vector<float> pcmData);

    bool open(const std::string & filename);

    bool close();
    
    VorbisEncoder(const unsigned int & channels,const unsigned int & sampleRate, const float & quality = 1.0f);
    ~VorbisEncoder();
};