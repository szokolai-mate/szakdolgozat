#pragma once

#include <string>

#include "iBuffer.h"

template <typename T>
class iLoader{
    //TODO: feaure evaluation
public:
    virtual void load(iBuffer<T> *) = 0;
    virtual void pause_load() = 0;
    virtual bool open(const std::string *) = 0;
};