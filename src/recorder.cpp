#include <Recorder.h>

#ifdef PA_WRITE_ERRORS
void Pa_error_occured_during_input(PaError err, std::string msg = "")
{
    Pa_Terminate();
    std::cerr << "An error occured while using the portaudio stream" << std::endl;
    std::cerr << "Error number: " << err << std::endl;
    std::cerr << "Error message: " << msg << std::endl
              << Pa_GetErrorText(err) << std::endl;
}
#endif

namespace Mixer{
template <>
bool Mixer::Recorder<float>::record(const float & seconds, bool oneBlock){
    PaStream *stream;
    PaError err;

    err = Pa_Initialize();
    if (err != paNoError)
    {
        #ifdef PA_WRITE_ERRORS
        Pa_error_occured_during_input(err);
        #endif
        return false;
    }

    PaStreamParameters inputParameters;
    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        #ifdef PA_WRITE_ERRORS
        Pa_error_occured_during_input(err,"No default input device!");
        #endif
        return false;
    }
    inputParameters.channelCount = 2;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    int framesPerSecond = 0;
    if(oneBlock){
        framesPerSecond = seconds * getChannels() * getSampleRate();
    }
    
    err = Pa_OpenStream(
        &stream,
        &inputParameters, 
        NULL, /*no output*/
        getSampleRate(),
        framesPerSecond, // NOTE: this had to be fixed for pulseaudio or popping was introduced. Not anymore!
        0,
        callbackFunction, /* the callback function. NULL for blocking API */
        this);    /* userData passed to the callback */

    if (err != paNoError)
    {
        #ifdef PA_WRITE_ERRORS
        Pa_error_occured_during_input(err);
        #endif
        return false;
    }
    err = Pa_StartStream( stream );
    if( err != paNoError ){
        #ifdef PA_WRITE_ERRORS
        Pa_error_occured_during_input(err);
        #endif
        return false;
    }

    while(buffer->size()<seconds*getChannels()*getSampleRate()){
        std::this_thread::sleep_for(std::chrono::duration<float, std::ratio<1, 1000>>(1));        
    }

    err = Pa_CloseStream( stream );
    if( err != paNoError ){
        #ifdef PA_WRITE_ERRORS
        Pa_error_occured_during_input(err);
        #endif
        return false;
    }
    Pa_Terminate();
    return true;
}
};

template <typename T>
int Mixer::Recorder<T>::callbackFunction(const void *inputBuffer, void *outputBuffer,
                                               unsigned long framesPerBuffer,
                                               const PaStreamCallbackTimeInfo *timeInfo,
                                               PaStreamCallbackFlags statusFlags,
                                               void *userData)
{
    /* Cast data passed through stream to our structure. */
    Mixer::Recorder<T> *recorder = (Mixer::Recorder<T>*)userData;
    T *in = (T *)inputBuffer;
    (void)outputBuffer; /* Prevent unused variable warning. */

    std::vector<T> data;
    data.reserve(framesPerBuffer*recorder->getChannels());
    for (int i = 0; i<framesPerBuffer;i++){
        for(int j = 0;j<recorder->getChannels();j++){
            data.push_back(*in++);
        }
    }
    recorder->buffer->put(data);

    return paContinue;
}

template class Mixer::Recorder<float>;