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
    vorbis_info vi;
    vorbis_comment vc;
    vorbis_dsp_state vd;
    vorbis_block vb;
  public:
    /*! \~english Add the ogg packet to header.
        \~hungarian Az ogg csomag hozzáadása a fejléchez.
        \~english \param packet the packet to add
                      \~hungarian \param packet a hozzáadandó csomag
        \~english \return error code from libvorbis
                \~hungarian \return hibakód libvorbis-tól
    */
    int addToHeader(ogg_packet & packet);
    /*! \~english Initialize the decoding state. The header needs to be complete.
        \~hungarian A dekódolási környezet inicializálása. A fejlécnek teljesnek kell lennie.
        \~english \return error code from libvorbis
                \~hungarian \return hibakód libvorbis-tól
    */
    int initDecoding();
    /*! \~english Get the comments from the Vorbis header. The header needs to be complete.
        \~hungarian Megadja a kommenteket a Vorbis fejlécből. A fejlécnek teljesnek kell lennie.
        \~english \return the comments
                \~hungarian \return a kommentek
    */
    std::vector<std::string> getComments() const;
    /*! \~english Get the vendor string from the Vorbis header. The header needs to be complete.
        \~hungarian Megadja a készítő szövegét a Vorbis fejlécből. A fejlécnek teljesnek kell lennie.
        \~english \return the vendor string
                \~hungarian \return a készítő szövege
    */
    std::string getVendor() const;
    /*! \~english Get the channel count from the Vorbis header. The header needs to be complete.
        \~hungarian Megadja a csatornák számát a Vorbis fejlécből. A fejlécnek teljesnek kell lennie.
        \~english \return the channel count
                \~hungarian \return a csatornák száma
    */
    unsigned int getChannels() const;
    /*! \~english Get the sample rate from the Vorbis header. The header needs to be complete.
        \~hungarian Megadja a mintavételezési sűrűséget a Vorbis fejlécből. A fejlécnek teljesnek kell lennie.
        \~english \return the sample rate
                \~hungarian \return a mintavételezési sűrűség
    */
    unsigned int getSampleRate() const;


    /*! \~english Decode an OGG packet into multiplexed PCM data.
        \~hungarian Egy OGG csomagot multiplexált PCM adatra dekódol.
        \~english \param packet the ogg packet to decode
                      \~hungarian \param packet a dekódolni kívánt ogg csomag
        \~english \return the decoded PCM data
                \~hungarian \return a dekódolt PCM adat
    */
    std::vector<float> decode(ogg_packet & packet);


    /*! \~english Clear the Vorbis decoding state.
        \~hungarian Kiüríri a Vorbis dekódolási környezetet.
    */
    void clear();

    VorbisDecoder();
    ~VorbisDecoder();
};