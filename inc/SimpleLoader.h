#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <future>

#include <ogg/ogg.h>
#include <vorbis/codec.h>

#include <iLoader.h>
#include <Utils.h>

template <typename T>
class simple_loader : public iLoader<T>{
private:
    std::ifstream infile;
public:
    void load(iBuffer<T> & _buffer , std::promise<audio_descriptor> _adp);
    bool open(const std::string & _filename);
	bool close();
    void pause_load();

	~simple_loader<T>();	
};