#pragma once

//! \~english Namespace for classes specializing in generating, mixing and playing audio streams.
//! \~hungarian Hangfolyamok készítésére, keverésére és lejátszására specializált osztályok névtere.
namespace Mixer
{
  //! \~english Interface describing basic audio player functionality.
  //! \~hungarian Alapvető zenelejátszó funkcionalitást leíró interfész.
class iPlayer
{
  public:
    /*!
      \~english Pause playback.Idempotent.
      \~hungarian Lejátszás szüneteltetése. Idempotens.
      \~english \return true on success, false on error
              \~hungarian \return igaz siker, hamis hiba esetén
    */
    virtual bool pause() = 0;
    /*!
      \~english Stops playback, resetting state.
      \~hungarian Megállítja a lejátszást, törölve a belső állapotot.
      \~english \return true on success, false on error
              \~hungarian \return igaz siker, hamis hiba esetén
    */
    virtual bool stop() = 0;
    /*!
      \~english Starts playback. Idempotent.
      \~hungarian Elindítja a lejátszást. Idempotens.
      \~english \return true on success, false on error
              \~hungarian \return igaz siker, hamis hiba esetén
    */
    virtual bool play() = 0;

    virtual ~iPlayer(){};
};
};