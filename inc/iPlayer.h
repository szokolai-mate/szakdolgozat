#pragma once

class iPlayer {
public:
    virtual bool pause() = 0;
    virtual bool stop() = 0;
    virtual bool play() = 0;

    virtual ~iPlayer(){};    
};