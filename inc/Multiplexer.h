#pragma once

#include <map>

#include <iSource.h>

namespace Mixer{
/*!
    \~english A class that will supply multiplexed audio data, which will have the number of channels desired.
    
    Assign mono audio data sources to each channel with setChannel() . Channels that do not have a source set will be silent.
    \~hungarian Multiplexált audió adatot szolgáltató osztály. A szolgáltatott adat a kért csatornaszámot tartalmazza.
    
    Monó audió forrásokat lehet csatlakoztatni egy csatornához a setChannel() -el. Azon csatornák, melyekre nincs forrás beállítva, némák lesznek.
*/
template <typename T>
class Multiplexer : public DataFlow::iSource<T>{
private:
    std::map<unsigned int, DataFlow::iSource<T> *> sources;
    unsigned int channelCount = 2;
public:
    std::vector<T> get(const unsigned int &amount);

    /*!
        \~english Set a channel to get it's data from an iSource.
        \~hungarian Beállítja az adott csatorna forrását.
        \~english \param channel the channel number
        \~hungarian \param channel a csatorna száma
        \~english \param source the source to attach to the channel
        \~hungarian \param source a csatornához csatolandó forrás
    */
    void setChannel(const unsigned int & channel, DataFlow::iSource<T> & source);

    /*!
        \~english Clear the channel of it's source, if there is one.
        \~hungarian A csatorna forrását kiüríti, ha volt neki.
        \~english \param channel the channel to clear
        \~hungarian \param channel a kiürítendő csatorna száma
    */
    void clearChannel(const unsigned int & channel);

    /*!
        \~english Set the number of channels.
        \~hungarian A csatornák számának megadása.
        \~english \param channels the number of channels
        \~hungarian \param channels a csatornák száma
    */
    void setChannelCount(unsigned int channels){
        channelCount = channels;
    }

    /*!
        \~english Get the number of channels.
        \~hungarian A csatornák számát adja vissza.
        \~english \return the number of channels
        \~hungarian \return a csatornák száma
    */
    unsigned int getChannelCount(){
        return channelCount;
    }

    /*!
        \~english Clears all channels of their source.
        \~hungarian Az összes csatorna forrását kiüríti.
    */
    void clear(){
        sources.clear();
    }

    /*!
        \~english Constructor. Initializes the channel count.
        \~hungarian Konstruktor. Inicializálja a csatornák számát.
         \~english \param channels the number of channels
        \~hungarian \param channels a csatornák száma
    */
    Multiplexer(unsigned int channels) : channelCount(channels) {}

    /*!
        \~english Default constructor. Channel count will be initialized to 2.
        \~hungarian Konstruktor. A csatornák számát 2-re inicializálja.
    */
    Multiplexer(){}
};
};