#pragma once

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vector>
#include <string>

/*! \~english Class used for decoding Vorbis encoded data.
    \~hungarian Vorbis kódolású adatot dekódoló osztály.
*/
class VorbisDecoder
{
  private:
    vorbis_info vi;      /* struct that stores all the static vorbis bitstream settings */
    vorbis_comment vc;   /* struct that stores all the bitstream user comments */
    vorbis_dsp_state vd; /* central working state for the packet->PCM decoder */
    vorbis_block vb;     /* local working space for packet->PCM decode */
  public:
    /*! \~english Add the ogg packet to header.
        \~hungarian Az ogg csomag hozzáadása a fejléchez.
        \param packet \~english the packet to add
                      \~hungarian a hozzáadandó csomag
        \return \~english error code from libvorbis
                \~hungarian hibakód libvorbis-tól
    */
    int addToHeader(ogg_packet & packet);
    /*! \~english Initialize the decoding state. The header needs to be complete.
        \~hungarian A dekódolási környezet inicializálása. A fejlécnek teljesnek kell lennie.
        \return \~english error code from libvorbis
                \~hungarian hibakód libvorbis-tól
    */
    int initDecoding();
    /*! \~english Get the comments from the Vorbis header. The header needs to be complete.
        \~hungarian Megadja a kommenteket a Vorbis fejlécből. A fejlécnek teljesnek kell lennie.
        \return \~english the comments
                \~hungarian a kommentek
    */
    std::vector<std::string> getComments() const;
    /*! \~english Get the vendor string from the Vorbis header. The header needs to be complete.
        \~hungarian Megadja a készítő szövegét a Vorbis fejlécből. A fejlécnek teljesnek kell lennie.
        \return \~english the vendor string
                \~hungarian a készítő szövege
    */
    std::string getVendor() const;
    /*! \~english Get the channel count from the Vorbis header. The header needs to be complete.
        \~hungarian Megadja a csatornák számát a Vorbis fejlécből. A fejlécnek teljesnek kell lennie.
        \return \~english the channel count
                \~hungarian a csatornák száma
    */
    unsigned int getChannels() const;
    /*! \~english Get the sample rate from the Vorbis header. The header needs to be complete.
        \~hungarian Megadja a mintavételezési sűrűséget a Vorbis fejlécből. A fejlécnek teljesnek kell lennie.
        \return \~english the sample rate
                \~hungarian a mintavételezési sűrűség
    */
    unsigned int getSampleRate() const;


    /*! \~english Decode an OGG packet into multiplexed PCM data.
        \~hungarian Egy OGG csomagot multiplexált PCM adatra dekódol.
        \param packet \~english the ogg packet to decode
                      \~hungarian a dekódolni kívánt ogg csomag
        \return \~english the decoded PCM data
                \~hungarian a dekódolt PCM adat
    */
    std::vector<float> decode(ogg_packet & packet);


    /*! \~english Clear the Vorbis decoding state.
        \~hungarian Kiüríri a Vorbis dekódolási környezetet.
    */
    void clear();

    VorbisDecoder();
    ~VorbisDecoder();
};