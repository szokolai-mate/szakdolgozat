#pragma once

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vector>
#include <string>
#include <fstream>
/*! \~english Class used for encoding data into OGG format, with Vorbis encoding.

    Lossy encoding. Uses variable bitrate encoding.

     Usage:

     - open file with open()
     - add comments to header with addComment()
     - initEncoding()
     - add audio data with add()
     - when finished use close()

    \~hungarian Vorbis kodekkel, OGG fomrátumba kódolásra használt osztály.

    Veszteséges kódolás. Változó bitrátát használ.

    Használat:

    - fájl megnyitása : open()
    - kommentek hozzáadása : addComment()
    - initEncoding()
    - adat hozzáadása : add()
    - fájl befejezése : close()
*/
class VorbisEncoder{
private:
    std::ofstream outfile;

    unsigned int channels, sampleRate;

    ogg_stream_state os;
    ogg_page         og;
    ogg_packet       op;

    vorbis_info vi;
    vorbis_comment vc;
    vorbis_dsp_state vd;
    vorbis_block vb;

    int eos = 0;
public:
     /*! \~english Add the comment to the header.

     Comments are a pair of strings, a key and a value.


        \~hungarian A komment hozzáadása a fejléchez.

        A kommentek két string-ből állnak, a kulcs és az érték.

        \~english \param key the key of the comment
                      \~hungarian \param key a hozzáadandó komment kulcsa
                      \~english \param value the value of the comment
                      \~hungarian \param value a hozzáadandó komment értéke
    */
    void addComment(std::string key,std::string value);

    /*! \~english Initialize the encoding state. The header needs to be complete.
        \~hungarian A kódolási környezet inicializálása. A fejlécnek teljesnek kell lennie.
        \~english \return error code from libvorbis
                \~hungarian \return hibakód libvorbis-tól
    */
    int initEncoding();

    /*!
        \~english Add audio data to the file. The data should be multiplex PCM with floating points.
        \~hungarian Audió adatot ad hozzá a fájlhoz. Az adatot multiplexált, lebegőpontos PCM adatnak tekintjük.
        \~english \param pcmData the data to add
        \~hungarian \param pcmData a hozzáadandó adat
    */
    void add(const std::vector<float> pcmData);

    /*!
        \~english Open a file to write the OGG data into.
        \~hungarian A kimenet fájl megnyitása amibe az OGG adatot fogjuk írni.
        \~english \param filename the name of the output file
        \~hungarian \param filename a kimeneti fájl neve
        \~hungarian \return a művelet sikeres volt-e
        \~english \return whether the operation was successful
    */
    bool open(const std::string & filename);

    /*!
        \~english Finalize the OGG data and close the opened file.
        \~hungarian Az OGG folyam és a kimenet fájl lezárása.
        \~hungarian \return a művelet sikeres volt-e
        \~english \return whether the operation was successful
    */
    bool close();
    

    /*!
        \~english Constructor. Requires the minimum information to begin encoding.
        \~hungarian Konstruktor. A kódolás megkezdéséhez minimálisan szükséges információját kéri.
        \~english \param channels number of channels
        \~hungarian \param channels a csatornák száma
        \~english \param sampleRate the sampling rate
        \~hungarian \param sampleRate a mintavételezési sűrűség 
        \~english \param quality the quality of the output file

        Possible range is [-0.1f;1.0f] , where -0.1f is smallest file and lowest quality
        \~hungarian \param quality a kimenet minősége

        A lehetséges intervallum [-0.1f;1.0f] , ahol -0.1f a legkisebb fájlméretet és hangminőséget jelent.
    */
    VorbisEncoder(const unsigned int & channels,const unsigned int & sampleRate, const float & quality = 1.0f);
    ~VorbisEncoder();
};