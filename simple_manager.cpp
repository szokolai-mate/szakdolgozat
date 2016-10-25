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