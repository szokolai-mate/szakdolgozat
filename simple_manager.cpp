//TODO: itt kell majd több folyamot összefésülni.
// ->next,next(amount),move semantics?


//lesz egy kevert buffer, ezt a többiből keveri össze. next;next(amount). repeatingbuffer-nek is kell lennie next(amount)-nak -> repeating is : IBuffer
//mit keverjen: csak valami iterálható adatszerkezet iBufferekből
// -> hogy kell minden collectiont elfogadtatni? iterátor?
// -> ezek szerint az időzítést nem itt kéne intézni, csak kívulről attach/detach buffereket
// --->bufferből ha elfogyik az adat, és a buffer már CLOSED! akkor detach. ilyenkor akár újrahasználhatnánk!
// -------->buffer pool?

#include "iManager.h"
#include "simple_player.cpp"

#include "utils.h"

template <class T>
class simple_manager : iManager< T > {
    public:
        bool play(){
           return this->player.play();
        }
        bool pause(){
            return this->player.pause();
        }
        bool stop(){
            return this->player.stop();
        }
        bool init(iBuffer<template_utils::extract_inner<T> > & _buffer,const audio_descriptor ad){
            return this->player.init(_buffer, ad);
        }


};