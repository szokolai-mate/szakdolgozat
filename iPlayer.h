#pragma once

class iPlayer {
public:
    //TODO: feature evaulation
    virtual bool play() = 0;
    virtual bool pause() = 0;
    virtual bool stop() = 0;
};