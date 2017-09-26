#pragma once

namespace Mixer
{
//! \~english Abstract class used to mark sources supplying audio data.
//! \~hungarian Audió adatot szolgáltató forrásokat megjelölő absztrakt osztály.
/*! \~english Contains properties which describes the audio stream supplied.
        \~hungarian A szolgáltatott audió adat leírásához szükséges tulajdonságokat tartalmazza.
    */
class AudioSource
{
  private:
    unsigned int channels = 2;
    unsigned int sampleRate = 48000;

  public:
    /*! \~english Get the channel count.
        \~hungarian A csatornák számát adja vissza.

        \return \~english the channel count
                \~hungarian a csatornák száma
    */
    unsigned int getChannels() { return channels; }
    /*! \~english Get the sample rate.
        \~hungarian A mintavételezés sűrűségét adja vissza.

        \return \~english the sample rate
                \~hungarian a mintavételezés sűrűsége
    */
    unsigned int getSampleRate() { return sampleRate; }

  protected:
    AudioSource(const unsigned int &channels, const unsigned int &sampleRate) : channels(channels), sampleRate(sampleRate) {}
};
};