#include <PortAudioBackend.h>

#ifdef PA_WRITE_ERRORS
void Pa_error_occured(PaError err, std::string msg = "")
{
    Pa_Terminate();
    std::cerr << "An error occured while using the portaudio stream" << std::endl;
    std::cerr << "Error number: " << err << std::endl;
    std::cerr << "Error message: " << msg << std::endl
              << Pa_GetErrorText(err) << std::endl;
}
#endif

template <typename T>
bool PortAudioBackend<T>::start()
{
    if (!stream)
    {
        return false;
    }
    if (Pa_IsStreamActive(stream))
    {
        return false;
    }
    err = Pa_StartStream(stream);
    if (err != paNoError)
    {
        Pa_error_occured(err);
        return false;
    }

    return true;
}

template <typename T>
bool PortAudioBackend<T>::stop()
{
    if (!stream)
    {
        return false;
    }
    if (Pa_IsStreamStopped(stream))
    {
        return false;
    }
    err = Pa_StopStream(stream);
    if (err != paNoError)
    {
        Pa_error_occured(err);
        return false;
    }
    return true;
}

template <typename T>
bool PortAudioBackend<T>::close()
{
    if (!stream)
    {
        return false;
    }
    err = Pa_StopStream(stream);
    if (err != paNoError)
    {
        Pa_error_occured(err);
        return false;
    }
    err = Pa_CloseStream(stream);
    if (err != paNoError)
    {
        Pa_error_occured(err);
        return false;
    }
    Pa_Terminate();

    return true;
}

template <>
bool PortAudioBackend<float>::init(DataFlow::iSource<float> &source, unsigned int channels, unsigned int sampleRate)
{
    this->stop();
    this->source = &source;
    
    PaStreamParameters outputParameters;

    err = Pa_Initialize();
    if (err != paNoError)
    {
        Pa_error_occured(err);
        return false;
    }

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice)
    {
        std::cerr << "Error: No default output device." << std::endl;
        if (err != paNoError)
        {
            Pa_error_occured(err);
            return false;
        }
    }
    outputParameters.channelCount = channels;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
        &stream,
        NULL, /* no input */
        &outputParameters,
        sampleRate,
        256 * 8, //fixed for pulseaudio, testing 0 for others
        //furcsa 1024-nél VAN underflow resume+playnél, de 2048-nál NINCS!  és ez a sine-ra is igaz ahol nincs buffer!
        paClipOff,        /* we won't output out of range samples so don't bother clipping them */
        callbackFunction, /* the callback function. NULL for blocking API */
        this->source);    /* userData passed to the callback */

    if (err != paNoError)
    {
        Pa_error_occured(err);
        return false;
    }
    return true;
}

template <>
int PortAudioBackend<float>::callbackFunction(const void *inputBuffer, void *outputBuffer,
                                               unsigned long framesPerBuffer,
                                               const PaStreamCallbackTimeInfo *timeInfo,
                                               PaStreamCallbackFlags statusFlags,
                                               void *userData)
{
    /* Cast data passed through stream to our structure. */
    DataFlow::iSource<float> *data = (DataFlow::iSource<float> *)userData;
    float *out = (float *)outputBuffer;
    (void)inputBuffer; /* Prevent unused variable warning. */


    //tmp FIXME
    //amount * 2 for stereo
    std::vector<float> vector = data->get(framesPerBuffer*2);
    std::copy(vector.begin(), vector.end(), out);
    return paContinue;
}

template class PortAudioBackend<float>;