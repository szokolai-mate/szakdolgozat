#pragma once

#include <fstream>
#include <string>
#include <vector>

#include <ogg/ogg.h>

#include <iSource.h>
#include <Utils.h>
#include <VorbisDecoder.h>

/*!
    \~english Class for reading OGG encoded files.
                Template type K is the decoder this instance will use.

                Usage:
                1. open file with open()
                2. initialize with init()
                3. the loader instance is ready to be read from with get(amount)
    \~hungarian OGG kódolt fájlok olvasására használható osztály.
                A K sablon paraméter a dekódolásra használandó osztály

                Használat:
                1. fájl megnyitása - open()            
                2. inicializálás - init()
                3. a példány készen áll az olvasásra - get()
*/
template <typename T,typename K>
class OggFileLoader : public DataFlow::iSource<T>{
private:
    std::ifstream infile;
    bool eos = true;
    char * buffer;

    ogg_sync_state   oy;
    ogg_stream_state os;

    ogg_page         currentPage;
    ogg_packet       currentPacket;

    K decoder;

    std::vector<T> overflow;

    bool streamInitialized = false;

    /*!
        \~english Initialize libogg.
        \~hungarian Libogg initializálása.

        \~english \return error code form libogg
                \~hungarian \return hibakód a libogg-tól
    */
    int initStream(ogg_page & page);
    /*!
        \~english Read data into libogg's internal buffer.
        \~hungarian A libogg belső pufferébe olvas adatot.

        \~english \return error code form libogg
                \~hungarian \return hibakód a libogg-tól
    */
    int read();
    /*!
        \~english Read the next page into this instance's currentPage.
        \~hungarian A következő oldalt olvassa be a példány currentPage változójába.

        \~english \return error code form libogg
                \~hungarian \return hibakód a libogg-tól
    */
    int readNextPage();
    /*!
        \~english Read the currentPage into libogg's internal buffer.
        \~hungarian A libogg belső pufferébe olvassa a currentPage oldalt.

        \~english \return error code form libogg
                \~hungarian \return hibakód a libogg-tól
    */
    int nextPage();
    /*!
        \~english Get the next packet from the page put into libogg.
        \~hungarian A következő csomagot olvassa ki a libogg-ba rakott oldalból.

        \~english \return error code form libogg
                \~hungarian \return hibakód a libogg-tól
    */
    int nextPacket();

public:
    /*!
        \~english Open file.
        \~hungarian Fájl megnyitása.

        \~english \return whether operation was successful
                \~hungarian \return sikeres volt-e a fájl megnyitása
    */
    bool open(const std::string & _filename);
    /*!
        \~english Initializes the instance for reading from.
        \~hungarian A példány felkészítése adatszolgáltatásra.

        \~english \return error code from decoder
                \~hungarian \return a dekódertól kapott hibakód
    */
    int init();  
     /*!
        \~english Close file and source.
        \~hungarian Fájl és forrás lezárása.

        \~english \return whether operation was successful
                \~hungarian \return sikeres volt-e a fájl lezárása
    */
    bool close();
    /*!
        \~english Clear the ogg stream state.
        \~hungarian Törli az ogg olvasás folyam belső állapotát.
    */
    void clear();
    std::vector<T> get(const unsigned int & amount);

    /*!
        \~english An acces point to the decoder instance used by this loader instance.
        \~hungarian Az ezen loader pélány által használt decoder példány hozzáférhető ezzel a metódussal.

        \~english \return a constant reference to the decoder instance
                \~hungarian \return egy konstans referencia a decoder példányhoz
    */
    const K & getDecoder();    

    OggFileLoader<T,K>();
	~OggFileLoader<T,K>();	
};