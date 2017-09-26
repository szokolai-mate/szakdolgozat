#pragma once

#include <vector>

#include <iSource.h>

namespace Mixer{
    //! \~english Interface for different audio backends.
    //! \~hungarian Különböző audió backendekhez tartozó interfész.
    template <typename T>
    class iAudioBackend{
    public:
        /*!
            \~english Initializes the audio backend into a playable state. Should not begin playing.
            \~hungarian Felkészíti a backendet lejáttszásra. Nem indítja el a lejátszást.
            \param source \~english the source of the audio data
                            \~hungarian az audió adat forrása
            \param channels \~english the channel count of the audio data
                            \~hungarian az audió adat csatornáinak száma
            \param sampleRate \~english the sample rate of the audio data
                            \~hungarian az audió adat mintavételezési sűrűsége 
            
            \return \~english wether initialization was successful
                    \~hungarian az inicializáció sikeres volt-e
        */
        virtual bool init(DataFlow::iSource<T> & source, const unsigned int & channels, const unsigned int & sampleRate) = 0;
        virtual ~iAudioBackend(){};
    };
};